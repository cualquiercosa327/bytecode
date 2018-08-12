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
#include <getopt.h>

void bytecode_runner_init(struct bytecode_runner *bcr)
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
        bytecode_runner_print_registers(bcr);
        bytecode_runner_print_stack(bcr);

        uint64_t raw_instr = fetch_instruction(bcr);
        struct bytecode_instruction instr = decode_instruction(raw_instr);

        printf("cycle %3" PRIu64 ": op = %-15s r1 = %s, r2 = %s\n",
                ++bcr->cycle_count,
                bytecode_opcode_str[instr.op],
                bytecode_register_str[instr.r1],
                bytecode_register_str[instr.r2]);

        execute_instruction(bcr, instr);

        if (bcr->single_step) getchar();
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

void bytecode_runner_print_registers(struct bytecode_runner *bcr)
{
    if (!bcr->verbose) return;

    static int num_col = 4;

    for (int i = 1; i <= BYTECODE_REGISTER_COUNT; ++i) {
        printf("%16s", bytecode_register_str[i - 1]);
        if ((i % num_col) == 0) {
            printf("\n");
            for (int j = 1; j <= num_col; ++j) {
                printf("%016llX ", bcr->reg[i - 1 - num_col + j]._u64);
            }
            printf("\n");
        }

        if (i == BYTECODE_REGISTER_COUNT) {
            int rem = i % num_col;
            printf("\n");
            for (int j = 1; j <= rem; ++j) {
                printf("%016llX ", bcr->reg[i - 1 - rem + j]._u64);
            }
            printf("\n");
        }
    }
}

void bytecode_runner_print_stack(struct bytecode_runner *bcr)
{
    if (!bcr->verbose) return;

    printf("stack [ ");
    for (int i = 0; i < bcr->reg[BYTECODE_REGISTER_RSP]._u64; ++i) {
        bytecode_value_print(stdout, &bcr->stack[i]);
        printf(" ");
    }
    printf("]\n");
}

void parse_arguments(int argc, char **argv, struct bytecode_runner *bcr)
{
    const char *short_opt = "vd";
    struct option long_opt[] = {
        { "--verbose", no_argument, NULL, 'v' },
        { "--debug", no_argument, NULL, 'd' },
        { NULL, 0, NULL, 0 }
    };

    int option;
    while ((option = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch (option) {
        case 'v': {
            bcr->verbose = true;
        } break;
        case 'd': {
            bcr->verbose = true;
            bcr->single_step = true;
        } break;
        }
    }
}

int main(int argc, char **argv)
{
    double a = 22.3f;
    double b =  3.2f;
    double c = 10.499999f;

    uint64_t main = 4;
    uint64_t program[] = {
        movi_reg_imm(BYTECODE_REGISTER_RAX, main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        enter(),
        movf_reg_imm(BYTECODE_REGISTER_RAX, a),
        addf_reg_imm(BYTECODE_REGISTER_RAX, b),
        subf_reg_imm(BYTECODE_REGISTER_RAX, c),
        mulf_reg_imm(BYTECODE_REGISTER_RAX, b),
        dec_reg(BYTECODE_REGISTER_RAX),
        movi_reg_imm(BYTECODE_REGISTER_R15, main),
        leave(),
    };

    struct bytecode_runner bcr = {};
    parse_arguments(argc, argv, &bcr);
    bytecode_runner_init(&bcr);
    bytecode_runner_run(&bcr, program);
    struct bytecode_value result = bytecode_runner_result(&bcr);
    bytecode_runner_destroy(&bcr);

    printf("result = ");
    bytecode_value_print(stdout, &result);
    printf("\n");

    return 0;
}
