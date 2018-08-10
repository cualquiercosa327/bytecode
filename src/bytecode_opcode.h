#ifndef BYTECODE_OPCODE_H
#define BYTECODE_OPCODE_H

#include "bytecode_instruction.h"

enum bytecode_opcode
{
    BYTECODE_OPCODE_HALT           = 0x00,

    BYTECODE_OPCODE_PUSH_INT       = 0x10,
    BYTECODE_OPCODE_PUSH_FLOAT     = 0x11,

    BYTECODE_OPCODE_ADD_INT_IMM    = 0x20,
    BYTECODE_OPCODE_ADD_FLOAT_IMM  = 0x30,

    BYTECODE_OPCODE_COUNT          = 0xFF
};

bytecode_instruction_handler_(exec_op_halt);
bytecode_instruction_handler_(exec_op_push_int);
bytecode_instruction_handler_(exec_op_push_float);
bytecode_instruction_handler_(exec_op_add_int_imm);
bytecode_instruction_handler_(exec_op_add_float_imm);

#endif
