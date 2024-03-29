#include "../../../src/bytecode_runner.h"
#include "../../../src/bytecode_opcode.h"

#include "../../../src/bytecode_instruction.h"
#include "../../../src/bytecode_instruction.c"

#include "../../../src/bytecode_executable.h"
#include "../../../src/bytecode_executable.c"

int main(int argc, char **argv)
{
    double a = 22.3f;
    double b =  3.2f;

    uint64_t __main = 4;
    uint64_t __add_floats = 38;

    char program_data[] = {
        'h','e','l','l','o',' ','w','o','r','l','d',' ','%','f','\n','\0',
        'p','r','i','n','t','f','\0',
        '/','u','s','r','/','l','i','b','/','l','i','b','c','.','d','y','l','i','b','\0'
    };

    uint64_t program_text[] = {
        mov_i64_reg_imm(BYTECODE_REGISTER_RAX, __main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        begin_call_frame(),
        mov_f64_reg_imm(BYTECODE_REGISTER_RDI, a),
        mov_f64_reg_imm(BYTECODE_REGISTER_RSI, b),
        add_i8_reg_imm(BYTECODE_REGISTER_RSP, 0x16),
        mov_i8_lcl_imm(0x0, 'r'),
        mov_i8_lcl_imm(0x1, 'r'),
        mov_i8_lcl_imm(0x2, 'i'),
        mov_i8_lcl_imm(0x3, 'n'),
        mov_i8_lcl_imm(0x4, 't'),
        mov_i8_lcl_imm(0x5, 'f'),
        mov_i8_lcl_imm(0x6, '\0'),
        lea_lcl_reg_imm(BYTECODE_REGISTER_RDX, 0),
        call_imm(__add_floats),
        end_call_frame(),
        ret(),
        begin_call_frame(),
        add_reg_reg(BYTECODE_REGISTER_RSI, BYTECODE_REGISTER_RDI),
        memw_i8_reg_imm(BYTECODE_REGISTER_RDX, 'p'),
        mov_reg_reg(BYTECODE_REGISTER_R11, BYTECODE_REGISTER_RDX),
        lea_bss_reg_imm(BYTECODE_REGISTER_RDI, 0),
        lea_bss_reg_imm(BYTECODE_REGISTER_R10, 23),
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
