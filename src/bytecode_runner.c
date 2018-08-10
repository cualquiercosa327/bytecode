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
    bcr->reg[BYTECODE_REGISTER_RIP] = create_u64_constant(0);
    bcr->reg[BYTECODE_REGISTER_RSP] = create_u64_constant(0);
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
    float a = 22.3f;
    float b =  3.2f;

    int64_t c = 0x1234567890abcdef;
    int64_t d = 0x1234567890abcdef;

    uint64_t program[] = {
        encode_instruction(BYTECODE_OPCODE_PUSH_INT), c,
        encode_instruction(BYTECODE_OPCODE_PUSH_INT), d,
        encode_instruction(BYTECODE_OPCODE_ADD_INT_IMM),
        encode_instruction_r1(BYTECODE_OPCODE_POP_REG, BYTECODE_REGISTER_RAX),
        encode_instruction(BYTECODE_OPCODE_PUSH_FLOAT), (uint64_t)(*(uint64_t *)&a),
        encode_instruction(BYTECODE_OPCODE_PUSH_FLOAT), (uint64_t)(*(uint64_t *)&b),
        encode_instruction(BYTECODE_OPCODE_ADD_FLOAT_IMM),
        encode_instruction(BYTECODE_OPCODE_HALT),
    };

    struct bytecode_runner bcr = {};
    bytecode_runner_init(&bcr, program);
    bytecode_runner_run(&bcr, program);
    struct bytecode_value result = bytecode_runner_result(&bcr);
    struct bytecode_value fval = bytecode_runner_pop_stack(&bcr);
    bytecode_runner_destroy(&bcr);

    print_bytecode_value(&fval, stdout);
    printf("\n");

    printf("result = ");
    print_bytecode_value(&result, stdout);
    printf("\n");

    return 0;
}
