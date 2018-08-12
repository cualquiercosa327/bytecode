#ifndef BYTECODE_INSTRUCTION_H
#define BYTECODE_INSTRUCTION_H

#define bytecode_instruction_handler_(name) void name(struct bytecode_runner *bcr, uint8_t op, uint8_t reg1, uint8_t reg2)
typedef bytecode_instruction_handler_(bytecode_instruction_handler);

struct bytecode_runner;

struct bytecode_instruction
{
    uint8_t op;
    uint8_t r1;
    uint8_t r2;
};

void execute_instruction(struct bytecode_runner *bcr, struct bytecode_instruction instr);
uint64_t fetch_instruction(struct bytecode_runner *bcr);

struct bytecode_instruction decode_instruction(uint64_t raw_instr);
uint64_t encode_instruction(uint8_t instr);
uint64_t encode_instruction_r1(uint8_t instr, uint8_t r1);
uint64_t encode_instruction_r2(uint8_t instr, uint8_t r1, uint8_t r2);

#define movu_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_MOVU_REG_IMM, reg), val
#define movi_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_MOVI_REG_IMM, reg), val
#define movf_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_MOVF_REG_IMM, reg), (uint64_t)(*(uint64_t *)&val)
#define mov_reg_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_MOV_REG_REG, reg1, reg2)

#define pushu_imm(val) encode_instruction(BYTECODE_OPCODE_PUSHU_IMM), val
#define pushi_imm(val) encode_instruction(BYTECODE_OPCODE_PUSHI_IMM), val
#define pushf_imm(val) encode_instruction(BYTECODE_OPCODE_PUSHF_IMM), (uint64_t)(*(uint64_t *)&val)
#define push_reg(reg) encode_instruction_r1(BYTECODE_OPCODE_PUSH_REG, reg)
#define pop_reg(reg) encode_instruction_r1(BYTECODE_OPCODE_POP_REG, reg)

#define addu_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_ADDU_REG_IMM, reg), val
#define addi_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_ADDI_REG_IMM, reg), val
#define addf_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_ADDF_REG_IMM, reg), (uint64_t)(*(uint64_t *)&val)
#define add_reg_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_ADD_REG_REG, reg1, reg2)

#define subu_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_SUBU_REG_IMM, reg), val
#define subi_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_SUBI_REG_IMM, reg), val
#define subf_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_SUBF_REG_IMM, reg), (uint64_t)(*(uint64_t *)&val)
#define sub_reg_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_SUB_REG_REG, reg1, reg2)

#define mulu_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_MULU_REG_IMM, reg), val
#define muli_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_MULI_REG_IMM, reg), val
#define mulf_reg_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_MULF_REG_IMM, reg), (uint64_t)(*(uint64_t *)&val)
#define mul_reg_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_MUL_REG_REG, reg1, reg2)

#define neg_reg(reg) encode_instruction_r1(BYTECODE_OPCODE_NEG_REG, reg)
#define inc_reg(reg) encode_instruction_r1(BYTECODE_OPCODE_INC_REG, reg)
#define dec_reg(reg) encode_instruction_r1(BYTECODE_OPCODE_DEC_REG, reg)
#define xor_reg_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_XOR_REG_REG, reg1, reg2)

#define call_imm(val) encode_instruction(BYTECODE_OPCODE_CALL_IMM), val
#define call_reg(reg) encode_instruction_r1(BYTECODE_OPCODE_CALL_REG, reg)

#define enter() encode_instruction(BYTECODE_OPCODE_ENTER)
#define leave() encode_instruction(BYTECODE_OPCODE_LEAVE)
#define store_local_imm(val, reg) encode_instruction_r1(BYTECODE_OPCODE_STORE_LOCAL_IMM, reg), val
#define store_local_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_STORE_LOCAL_REG, reg1, reg2)
#define load_local_imm(reg, val) encode_instruction_r1(BYTECODE_OPCODE_LOAD_LOCAL_IMM, reg), val
#define load_local_reg(reg1, reg2) encode_instruction_r2(BYTECODE_OPCODE_LOAD_LOCAL_REG, reg1, reg2)

#define nop() encode_instruction(BYTECODE_OPCODE_NOP)
#define halt() encode_instruction(BYTECODE_OPCODE_HALT)

#endif
