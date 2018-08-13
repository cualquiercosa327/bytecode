#ifndef BYTECODE_RUNNER_H
#define BYTECODE_RUNNER_H

#include "bytecode_value.h"

#include <stdint.h>
#include <stdbool.h>

static const char *bytecode_register_str[] =
{
    "RIP",

    "RAX",
    "RBX",
    "RCX",
    "RDX",
    "RBP",
    "RSP",
    "RSI",
    "RDI",

    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15",

    "COUNT"
};

enum bytecode_register
{
    BYTECODE_REGISTER_RIP,

    BYTECODE_REGISTER_RAX,
    BYTECODE_REGISTER_RBX,
    BYTECODE_REGISTER_RCX,
    BYTECODE_REGISTER_RDX,
    BYTECODE_REGISTER_RBP,
    BYTECODE_REGISTER_RSP,
    BYTECODE_REGISTER_RSI,
    BYTECODE_REGISTER_RDI,

    BYTECODE_REGISTER_R8,
    BYTECODE_REGISTER_R9,
    BYTECODE_REGISTER_R10,
    BYTECODE_REGISTER_R11,
    BYTECODE_REGISTER_R12,
    BYTECODE_REGISTER_R13,
    BYTECODE_REGISTER_R14,
    BYTECODE_REGISTER_R15,

    BYTECODE_REGISTER_COUNT
};

struct bytecode_runner
{
    bool verbose;
    bool single_step;

    bool is_running;
    uint64_t cycle_count;

    char *data;
    uint32_t data_size;

    uint64_t *text;
    uint32_t stack_size;
    struct bytecode_value *stack;

    struct bytecode_value reg[BYTECODE_REGISTER_COUNT];
};

struct bytecode_executable;

void bytecode_runner_init(struct bytecode_runner *bcr, struct bytecode_executable *program);
void bytecode_runner_destroy(struct bytecode_runner *bcr);

void bytecode_runner_run(struct bytecode_runner *bcr);
struct bytecode_value bytecode_runner_result(struct bytecode_runner *bcr);

void bytecode_runner_push_stack(struct bytecode_runner *bcr, struct bytecode_value value);
struct bytecode_value bytecode_runner_pop_stack(struct bytecode_runner *bcr);

void bytecode_runner_print_registers(struct bytecode_runner *bcr);
void bytecode_runner_print_stack(struct bytecode_runner *bcr);

#endif
