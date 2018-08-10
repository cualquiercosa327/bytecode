#ifndef BYTECODE_RUNNER_H
#define BYTECODE_RUNNER_H

#include "bytecode_value.h"

#include <stdint.h>
#include <stdbool.h>

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
    bool is_running;
    uint64_t cycle_count;

    uint64_t *text;
    uint32_t stack_head;
    uint32_t stack_size;
    struct bytecode_value *stack;

    struct bytecode_value reg[BYTECODE_REGISTER_COUNT];
};

void bytecode_runner_init(struct bytecode_runner *bcr, uint64_t *program);
void bytecode_runner_destroy(struct bytecode_runner *bcr);

void bytecode_runner_run(struct bytecode_runner *bcr, uint64_t *program);
struct bytecode_value bytecode_runner_result(struct bytecode_runner *bcr);

void bytecode_runner_push_stack(struct bytecode_runner *bcr, struct bytecode_value value);
struct bytecode_value bytecode_runner_pop_stack(struct bytecode_runner *bcr);

#endif
