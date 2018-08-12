#include "bytecode_runner.h"
#include "bytecode_opcode.h"

#include "bytecode_value.h"
#include "bytecode_value.c"

#include "bytecode_instruction.h"
#include "bytecode_instruction.c"

#include "bytecode_instruction_handler.h"
#include "bytecode_instruction_handler.c"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void bytecode_runner_init(struct bytecode_runner *bcr, uint64_t *program)
{
    bcr->cycle_count = 0;
    bcr->stack_size = 200;
    bcr->stack = malloc(bcr->stack_size * sizeof(struct bytecode_value));
    bcr->reg[BYTECODE_REGISTER_RIP] = bytecode_value_create_u64(0);
    bcr->reg[BYTECODE_REGISTER_RSP] = bytecode_value_create_u64(0);
}

void bytecode_runner_destroy(struct bytecode_runner *bcr)
{
    free(bcr->stack);
    memset(bcr, 0, sizeof(struct bytecode_runner));
}

void bytecode_runner_run(struct bytecode_runner *bcr, uint64_t *program)
{
    bcr->text = program;
    bcr->is_running = true;

    while (bcr->is_running) {
        uint64_t raw_instr = fetch_instruction(bcr);
        struct bytecode_instruction instr = decode_instruction(raw_instr);

        printf("cycle %3" PRIu64 ": op = %2X, r1 = %2d, r2 = %2d\n",
                ++bcr->cycle_count, instr.op, instr.r1, instr.r2);

        execute_instruction(bcr, instr);
    }
}

struct bytecode_value bytecode_runner_result(struct bytecode_runner *bcr)
{
    return bcr->reg[BYTECODE_REGISTER_RAX];
}

void bytecode_runner_push_stack(struct bytecode_runner *bcr, struct bytecode_value value)
{
    bcr->stack[bcr->reg[BYTECODE_REGISTER_RSP]._u64++] = value;
    assert(bcr->reg[BYTECODE_REGISTER_RSP]._u64 < bcr->stack_size);
}

struct bytecode_value bytecode_runner_pop_stack(struct bytecode_runner *bcr)
{
    assert(bcr->reg[BYTECODE_REGISTER_RSP]._u64 > 0);
    return bcr->stack[--bcr->reg[BYTECODE_REGISTER_RSP]._u64];
}

int main(int argc, char **argv)
{
    double a = 22.3f;
    double b =  3.2f;

    int64_t c = 0x1234567890abcdef;
    int64_t d = 0x1234567890abcdef;

    uint64_t program[] = {
        movi_reg_imm(BYTECODE_REGISTER_RAX, c),
        movi_reg_imm(BYTECODE_REGISTER_RBX, d),
        add_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_RBX),
        movf_reg_imm(BYTECODE_REGISTER_RAX, a),
        addf_reg_imm(BYTECODE_REGISTER_RAX, b),
        halt,
    };

    struct bytecode_runner bcr = {};
    bytecode_runner_init(&bcr, program);
    bytecode_runner_run(&bcr, program);
    struct bytecode_value result = bytecode_runner_result(&bcr);
    bytecode_runner_destroy(&bcr);

    printf("result = ");
    bytecode_value_print(stdout, &result);
    printf("\n");

    return 0;
}
