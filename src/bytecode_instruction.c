#include "bytecode_instruction.h"
#include "bytecode_runner.h"

uint64_t fetch_instruction(struct bytecode_runner *bcr)
{
    return bcr->text[bcr->reg[BYTECODE_REGISTER_RIP]._u64++];
}

struct bytecode_instruction decode_instruction(uint64_t raw_instr)
{
    struct bytecode_instruction instr;
    instr.op = (raw_instr & 0xff00000000000000) >> 56;
    instr.r1 = (raw_instr & 0x00f0000000000000) >> 52;
    instr.r2 = (raw_instr & 0x000f000000000000) >> 48;
    return instr;
}

uint64_t encode_instruction(uint8_t instr)
{
    uint64_t result = (uint64_t)instr << 56;
    return result;
}

uint64_t encode_instruction_r1(uint8_t instr, uint8_t r1)
{
    uint64_t result = ((uint64_t)instr << 56) | ((uint64_t)r1 << 52);
    return result;
}

uint64_t encode_instruction_r2(uint8_t instr, uint8_t r1, uint8_t r2)
{
    uint64_t result = ((uint64_t)instr << 56) | ((uint64_t)r1 << 52) | ((uint64_t)r2 << 48);
    return result;
}
