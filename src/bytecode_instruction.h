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

#endif
