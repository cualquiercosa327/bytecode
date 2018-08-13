#include "bytecode_instruction_handler.h"
#include "bytecode_runner.h"
#include "bytecode_value.h"

#include <assert.h>

bytecode_instruction_handler_(exec_op_halt)
{
    bcr->is_running = false;
}

bytecode_instruction_handler_(exec_op_nop)
{
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
    bytecode_value_add(&bcr->reg[reg1], u64_constant);
}

bytecode_instruction_handler_(exec_op_addi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_value_add(&bcr->reg[reg1], s64_constant);
}

bytecode_instruction_handler_(exec_op_addf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_value_add(&bcr->reg[reg1], f64_constant);
}

bytecode_instruction_handler_(exec_op_add_reg_reg)
{
    bytecode_value_add(&bcr->reg[reg1], bcr->reg[reg2]);
}

bytecode_instruction_handler_(exec_op_subu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_value_sub(&bcr->reg[reg1], u64_constant);
}

bytecode_instruction_handler_(exec_op_subi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_value_sub(&bcr->reg[reg1], s64_constant);
}

bytecode_instruction_handler_(exec_op_subf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_value_sub(&bcr->reg[reg1], f64_constant);
}

bytecode_instruction_handler_(exec_op_sub_reg_reg)
{
    bytecode_value_sub(&bcr->reg[reg1], bcr->reg[reg2]);
}

bytecode_instruction_handler_(exec_op_mulu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_value_mul(&bcr->reg[reg1], u64_constant);
}

bytecode_instruction_handler_(exec_op_muli_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_value_mul(&bcr->reg[reg1], s64_constant);
}

bytecode_instruction_handler_(exec_op_mulf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_value_mul(&bcr->reg[reg1], f64_constant);
}

bytecode_instruction_handler_(exec_op_mul_reg_reg)
{
    bytecode_value_mul(&bcr->reg[reg1], bcr->reg[reg2]);
}

bytecode_instruction_handler_(exec_op_neg_reg)
{
    bytecode_value_neg(&bcr->reg[reg1]);
}

bytecode_instruction_handler_(exec_op_inc_reg)
{
    struct bytecode_value one = bytecode_value_constant_one(bcr->reg[reg1]);
    bytecode_value_add(&bcr->reg[reg1], one);
}

bytecode_instruction_handler_(exec_op_dec_reg)
{
    struct bytecode_value one = bytecode_value_constant_one(bcr->reg[reg1]);
    bytecode_value_sub(&bcr->reg[reg1], one);
}

bytecode_instruction_handler_(exec_op_xor_reg_reg)
{
    bcr->reg[reg1]._u64 ^= bcr->reg[reg2]._u64;
}

bytecode_instruction_handler_(exec_op_call_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_runner_push_stack(bcr, bcr->reg[BYTECODE_REGISTER_RIP]);
    bcr->reg[BYTECODE_REGISTER_RIP] = u64_constant;
}

bytecode_instruction_handler_(exec_op_call_reg)
{
    bytecode_runner_push_stack(bcr, bcr->reg[BYTECODE_REGISTER_RIP]);
    bcr->reg[BYTECODE_REGISTER_RIP] = bcr->reg[reg1];
}

bytecode_instruction_handler_(exec_op_enter)
{
    bytecode_runner_push_stack(bcr, bcr->reg[BYTECODE_REGISTER_RBP]);
    bcr->reg[BYTECODE_REGISTER_RBP] = bcr->reg[BYTECODE_REGISTER_RSP];
}

bytecode_instruction_handler_(exec_op_leave)
{
    bcr->reg[BYTECODE_REGISTER_RSP] = bcr->reg[BYTECODE_REGISTER_RBP];
    bcr->reg[BYTECODE_REGISTER_RBP] = bytecode_runner_pop_stack(bcr);
    bcr->reg[BYTECODE_REGISTER_RIP] = bytecode_runner_pop_stack(bcr);
}

bytecode_instruction_handler_(exec_op_store_local_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + u64_constant._u64] = bcr->reg[reg1];
}

bytecode_instruction_handler_(exec_op_store_local_reg)
{
    bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + bcr->reg[reg1]._u64] = bcr->reg[reg2];
}

bytecode_instruction_handler_(exec_op_load_local_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bcr->reg[reg1] = bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + u64_constant._u64];
}

bytecode_instruction_handler_(exec_op_load_local_reg)
{
    bcr->reg[reg1] = bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + bcr->reg[reg2]._u64];
}

bytecode_instruction_handler_(exec_op_lea_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    char *effective_address = bcr->data + u64_constant._u64;
    assert(effective_address < bcr->data + bcr->data_size);
    bcr->reg[reg1] = bytecode_value_create_ptr((uint64_t)effective_address);
}

bytecode_instruction_handler_(exec_op_lea_reg_reg)
{
    bcr->reg[reg1] = bytecode_value_create_ptr(bcr->reg[reg2]._u64);
}
