#ifndef BYTECODE_INSTRUCTION_HANDLERS
#define BYTECODE_INSTRUCTION_HANDLERS

#include "bytecode_instruction.h"

bytecode_instruction_handler_(exec_op_halt);
bytecode_instruction_handler_(exec_op_movu_reg_imm);
bytecode_instruction_handler_(exec_op_movi_reg_imm);
bytecode_instruction_handler_(exec_op_movf_reg_imm);
bytecode_instruction_handler_(exec_op_mov_reg_reg);
bytecode_instruction_handler_(exec_op_pushu_imm);
bytecode_instruction_handler_(exec_op_pushi_imm);
bytecode_instruction_handler_(exec_op_pushf_imm);
bytecode_instruction_handler_(exec_op_push_reg);
bytecode_instruction_handler_(exec_op_pop_reg);
bytecode_instruction_handler_(exec_op_addu_reg_imm);
bytecode_instruction_handler_(exec_op_addi_reg_imm);
bytecode_instruction_handler_(exec_op_addf_reg_imm);
bytecode_instruction_handler_(exec_op_add_reg_reg);
bytecode_instruction_handler_(exec_op_subu_reg_imm);
bytecode_instruction_handler_(exec_op_subi_reg_imm);
bytecode_instruction_handler_(exec_op_subf_reg_imm);
bytecode_instruction_handler_(exec_op_sub_reg_reg);
bytecode_instruction_handler_(exec_op_mulu_reg_imm);
bytecode_instruction_handler_(exec_op_muli_reg_imm);
bytecode_instruction_handler_(exec_op_mulf_reg_imm);
bytecode_instruction_handler_(exec_op_mul_reg_reg);

#endif
