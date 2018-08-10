#include "bytecode_opcode.h"
#include "bytecode_runner.h"
#include "bytecode_value.h"
#include "bytecode_instruction.h"

#include <assert.h>

bytecode_instruction_handler_(exec_op_halt)
{
    bcr->is_running = false;
}

bytecode_instruction_handler_(exec_op_push_int)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = create_s64_constant(constant);
    push_stack(bcr, s64_constant);
}

bytecode_instruction_handler_(exec_op_push_float)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f32_constant = create_f32_constant(constant);
    push_stack(bcr, f32_constant);
}

bytecode_instruction_handler_(exec_op_add_int_imm)
{
    struct bytecode_value rhs = pop_stack(bcr);
    struct bytecode_value lhs = pop_stack(bcr);
    assert(lhs.kind == rhs.kind);
    lhs._s64 += rhs._s64;
    push_stack(bcr, lhs);
}

bytecode_instruction_handler_(exec_op_add_float_imm)
{
    struct bytecode_value rhs = pop_stack(bcr);
    struct bytecode_value lhs = pop_stack(bcr);
    assert(lhs.kind == rhs.kind);
    lhs._f32 += rhs._f32;
    push_stack(bcr, lhs);
}
