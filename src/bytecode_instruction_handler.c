#include "bytecode_instruction_handler.h"
#include "bytecode_runner.h"
#include "bytecode_value.h"

#include <assert.h>

bytecode_instruction_handler_(exec_op_halt)
{
    bcr->is_running = false;
}

bytecode_instruction_handler_(exec_op_movu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bcr->reg[reg1] = u64_constant;
}

bytecode_instruction_handler_(exec_op_movi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bcr->reg[reg1] = s64_constant;
}

bytecode_instruction_handler_(exec_op_movf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bcr->reg[reg1] = f64_constant;
}

bytecode_instruction_handler_(exec_op_mov_reg_reg)
{
    bcr->reg[reg1] = bcr->reg[reg2];
}

bytecode_instruction_handler_(exec_op_pushu_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_runner_push_stack(bcr, u64_constant);
}

bytecode_instruction_handler_(exec_op_pushi_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_runner_push_stack(bcr, s64_constant);
}

bytecode_instruction_handler_(exec_op_pushf_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_runner_push_stack(bcr, f64_constant);
}

bytecode_instruction_handler_(exec_op_push_reg)
{
    bytecode_runner_push_stack(bcr, bcr->reg[reg1]);
}

bytecode_instruction_handler_(exec_op_pop_reg)
{
    bcr->reg[reg1] = bytecode_runner_pop_stack(bcr);
}

bytecode_instruction_handler_(exec_op_addu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    assert(bcr->reg[reg1].kind == u64_constant.kind);
    bcr->reg[reg1]._u64 += u64_constant._u64;
}

bytecode_instruction_handler_(exec_op_addi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    assert(bcr->reg[reg1].kind == s64_constant.kind);
    bcr->reg[reg1]._s64 += s64_constant._s64;
}

bytecode_instruction_handler_(exec_op_addf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    assert(bcr->reg[reg1].kind == f64_constant.kind);
    bcr->reg[reg1]._f64 += f64_constant._f64;
}

bytecode_instruction_handler_(exec_op_add_reg_reg)
{
    bytecode_value_add(&bcr->reg[reg1], bcr->reg[reg2]);
}
