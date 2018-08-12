#ifndef BYTECODE_OPCODE_H
#define BYTECODE_OPCODE_H

enum bytecode_opcode
{
    BYTECODE_OPCODE_HALT           = 0x00,

    BYTECODE_OPCODE_MOVU_REG_IMM   = 0x01,
    BYTECODE_OPCODE_MOVI_REG_IMM   = 0x02,
    BYTECODE_OPCODE_MOVF_REG_IMM   = 0x03,
    BYTECODE_OPCODE_MOV_REG_REG    = 0x04,

    BYTECODE_OPCODE_PUSHU_IMM      = 0x10,
    BYTECODE_OPCODE_PUSHI_IMM      = 0x11,
    BYTECODE_OPCODE_PUSHF_IMM      = 0x12,
    BYTECODE_OPCODE_PUSH_REG       = 0x13,
    BYTECODE_OPCODE_POP_REG        = 0x14,

    BYTECODE_OPCODE_ADDU_REG_IMM   = 0x30,
    BYTECODE_OPCODE_ADDI_REG_IMM   = 0x31,
    BYTECODE_OPCODE_ADDF_REG_IMM   = 0x32,
    BYTECODE_OPCODE_ADD_REG_REG    = 0x33,

    BYTECODE_OPCODE_SUBU_REG_IMM   = 0x34,
    BYTECODE_OPCODE_SUBI_REG_IMM   = 0x35,
    BYTECODE_OPCODE_SUBF_REG_IMM   = 0x36,
    BYTECODE_OPCODE_SUB_REG_REG    = 0x37,

    BYTECODE_OPCODE_MULU_REG_IMM   = 0x38,
    BYTECODE_OPCODE_MULI_REG_IMM   = 0x39,
    BYTECODE_OPCODE_MULF_REG_IMM   = 0x3A,
    BYTECODE_OPCODE_MUL_REG_REG    = 0x3B,

    BYTECODE_OPCODE_NEG_REG        = 0x3C,
    BYTECODE_OPCODE_INC_REG        = 0x3D,
    BYTECODE_OPCODE_DEC_REG        = 0x3E,

    BYTECODE_OPCODE_CALL_IMM       = 0x50,
    BYTECODE_OPCODE_CALL_REG       = 0x51,

    BYTECODE_OPCODE_ENTER          = 0x60,
    BYTECODE_OPCODE_LEAVE          = 0x61,

    BYTECODE_OPCODE_COUNT          = 0xFF
};

#endif
