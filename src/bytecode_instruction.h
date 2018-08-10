#ifndef BYTECODE_INSTRUCTION_H
#define BYTECODE_INSTRUCTION_H

struct bytecode_runner;

struct bytecode_instruction
{
    uint8_t op;
    uint8_t r1;
    uint8_t r2;
};

uint64_t fetch_instruction(struct bytecode_runner *bcr);

struct bytecode_instruction decode_instruction(uint64_t raw_instr);

uint64_t encode_instruction(uint8_t instr);
uint64_t encode_instruction_r1(uint8_t instr, uint8_t r1);
uint64_t encode_instruction_r2(uint8_t instr, uint8_t r1, uint8_t r2);

#endif
