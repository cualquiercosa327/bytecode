#include "../../../src/bytecode_runner.h"
#include "../../../src/bytecode_opcode.h"

#include "../../../src/bytecode_instruction.h"
#include "../../../src/bytecode_instruction.c"

#include "../../../src/bytecode_executable.h"
#include "../../../src/bytecode_executable.c"

int main(int argc, char **argv)
{
    uint64_t __main = 4;

    char program_data[] = {
        '%','d',' ','*',' ','%','d',' ','=',' ','%','d','\n','\0',
        'p','r','i','n','t','f','\0',
        '/','u','s','r','/','l','i','b','/','l','i','b','c','.','d','y','l','i','b','\0'
    };

    uint64_t program_text[] = {
        movi_reg_imm(BYTECODE_REGISTER_RAX, __main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        enter(),
        movi_reg_imm(BYTECODE_REGISTER_RSI, 7),
        movi_reg_imm(BYTECODE_REGISTER_RDX, 4),
        mov_reg_reg(BYTECODE_REGISTER_RCX, BYTECODE_REGISTER_RSI),
        mul_reg_reg(BYTECODE_REGISTER_RCX, BYTECODE_REGISTER_RDX),
        lea_reg_imm(BYTECODE_REGISTER_RDI, 0),
        call_foreign(14, 21, 4, BYTECODE_VALUE_S32),
        leave(),
    };

    struct bytecode_executable program = {
        .header = {
            .stack_size = 200,
            .data_size = sizeof(program_data),
            .text_size = sizeof(program_text)
        },
        .data_segment = program_data,
        .text_segment = program_text
    };

    if (bytecode_write_executable("./sample.bcr", program)) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
