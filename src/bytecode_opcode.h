#ifndef BYTECODE_OPCODE_H
#define BYTECODE_OPCODE_H

enum bytecode_opcode
{
    BYTECODE_OPCODE_HALT           = 0x00,

    BYTECODE_OPCODE_PUSH_INT       = 0x10,
    BYTECODE_OPCODE_PUSH_FLOAT     = 0x11,

    BYTECODE_OPCODE_ADD_INT_IMM    = 0x20,
    BYTECODE_OPCODE_ADD_FLOAT_IMM  = 0x30,

    BYTECODE_OPCODE_COUNT          = 0xFF
};

#endif