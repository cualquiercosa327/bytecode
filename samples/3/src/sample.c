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
        'p','r','i','n','t','f','\0',
        '/','u','s','r','/','l','i','b','/','l','i','b','c','.','d','y','l','i','b','\0'
    };

    uint64_t program_text[] = {
        mov_i64_reg_imm(BYTECODE_REGISTER_RAX, __main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        begin_call_frame(),
        add_i8_reg_imm(BYTECODE_REGISTER_RSP, 0x16),
        mov_i8_lcl_imm(0, 'h'),
        mov_i8_lcl_imm(1, 'e'),
        mov_i8_lcl_imm(2, 'l'),
        mov_i8_lcl_imm(3, 'l'),
        mov_i8_lcl_imm(4, 'o'),
        mov_i8_lcl_imm(5, ','),
        mov_i8_lcl_imm(6, ' '),
        mov_i8_lcl_imm(7, 'w'),
        mov_i8_lcl_imm(8, 'o'),
        mov_i8_lcl_imm(9, 'r'),
        mov_i8_lcl_imm(10, 'l'),
        mov_i8_lcl_imm(11, 'd'),
        mov_i8_lcl_imm(12, '\n'),
        mov_i8_lcl_imm(13, '\0'),
        lea_lcl_reg_imm(BYTECODE_REGISTER_RDI, 0),
        lea_bss_reg_imm(BYTECODE_REGISTER_R10, 7),
        lea_bss_reg_imm(BYTECODE_REGISTER_R11, 0),
        push_reg(BYTECODE_REGISTER_R10),
        push_reg(BYTECODE_REGISTER_R11),
        call_foreign(1, BYTECODE_REGISTER_KIND_I32),
        end_call_frame(),
        ret()
    };

    struct bytecode_header program_header = {
        .magic = { 'b', 'c', 'r' },
        .abi_version = 0x1,
        .stack_size = 200,
        .data_size = sizeof(program_data),
        .text_size = sizeof(program_text)
    };

    struct bytecode_executable program = {
        .header = &program_header,
        .data_segment = program_data,
        .text_segment = program_text
    };

    if (bytecode_write_executable("./sample.bcr", &program)) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
