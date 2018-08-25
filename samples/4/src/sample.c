#include "../../../src/bytecode_runner.h"
#include "../../../src/bytecode_opcode.h"

#include "../../../src/bytecode_instruction.h"
#include "../../../src/bytecode_instruction.c"

#include "../../../src/bytecode_executable.h"
#include "../../../src/bytecode_executable.c"

int main(int argc, char **argv)
{
    uint64_t __main = 4;

    float a = 5.0f;
    float b = 2.0f;

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
        mov_i8_lcl_imm(0, '%'),
        mov_i8_lcl_imm(1, '.'),
        mov_i8_lcl_imm(2, '2'),
        mov_i8_lcl_imm(3, 'f'),
        mov_i8_lcl_imm(4, '\n'),
        mov_i8_lcl_imm(5, '\0'),
        lea_lcl_reg_imm(BYTECODE_REGISTER_RDI, 0),
        mov_f32_reg_imm(BYTECODE_REGISTER_RSI, a),
        div_f32_reg_imm(BYTECODE_REGISTER_RSI, b),
        lea_bss_reg_imm(BYTECODE_REGISTER_R10, 7),
        lea_bss_reg_imm(BYTECODE_REGISTER_R11, 0),
        push_reg(BYTECODE_REGISTER_R10),
        push_reg(BYTECODE_REGISTER_R11),
        call_foreign(2, BYTECODE_REGISTER_KIND_I32),
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
