#include "bytecode_runner.h"

#include "bytecode_value.h"
#include "bytecode_value.c"

#include "bytecode_instruction.h"
#include "bytecode_instruction.c"

#include "bytecode_opcode.h"
#include "bytecode_opcode.c"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

void bytecode_runner_init(struct bytecode_runner *bcr, uint64_t *program)
{
    bcr->stack_head = 0;
    bcr->stack_size = 200;
    bcr->stack = malloc(bcr->stack_size * sizeof(struct bytecode_value));

    bcr->reg[BYTECODE_REGISTER_RIP] = create_u64_constant(0);

    bcr->text = program;
    bcr->cycle_count = 0;
    bcr->is_running = true;
}

void bytecode_runner_destroy(struct bytecode_runner *bcr)
{
    free(bcr->stack);
    memset(bcr, 0, sizeof(struct bytecode_runner));
}

struct bytecode_value bytecode_runner_result(struct bytecode_runner *bcr)
{
    return bcr->reg[BYTECODE_REGISTER_RAX];
}

void push_stack(struct bytecode_runner *bcr, struct bytecode_value value)
{
    bcr->stack[bcr->stack_head++] = value;
    assert(bcr->stack_head < bcr->stack_size);
}

struct bytecode_value pop_stack(struct bytecode_runner *bcr)
{
    assert(bcr->stack_head > 0);
    return bcr->stack[--bcr->stack_head];
}

void do_cycle(struct bytecode_runner *bcr)
{
    uint64_t raw_instr = fetch_instruction(bcr);
    struct bytecode_instruction instr = decode_instruction(raw_instr);

    printf("cycle %3" PRIu64 ": op = %2X, r1 = %2d, r2 = %2d\n",
            ++bcr->cycle_count, instr.op, instr.r1, instr.r2);

    execute_instruction(bcr, instr);
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
        encode_instruction(BYTECODE_OPCODE_PUSH_FLOAT), (uint64_t)(*(uint64_t *)&a),
        encode_instruction(BYTECODE_OPCODE_PUSH_FLOAT), (uint64_t)(*(uint64_t *)&b),
        encode_instruction(BYTECODE_OPCODE_ADD_FLOAT_IMM),
        encode_instruction(BYTECODE_OPCODE_HALT),
    };

    struct bytecode_runner bcr = {};
    bytecode_runner_init(&bcr, program);

    while (bcr.is_running) {
        do_cycle(&bcr);
    }

    struct bytecode_value fval = pop_stack(&bcr);
    printf("head = %.2f\n", fval._f32);

    struct bytecode_value ival = pop_stack(&bcr);
    printf("head = %" PRId64 "\n", ival._s64);

    bytecode_runner_destroy(&bcr);

    return 0;
}
