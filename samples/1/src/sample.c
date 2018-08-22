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
    double c = 10.499999f;

    uint64_t __main = 4;
    uint64_t __add_floats = 35;

    char program_data[] = {
        'h','e','l','l','o','\n','\0',
        'p','r','i','n','t','f','\0',
        '/','u','s','r','/','l','i','b','/','l','i','b','c','.','d','y','l','i','b','\0'
    };

    uint64_t program_text[] = {
        movi_reg_imm(BYTECODE_REGISTER_RAX, __main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        enter(),
        addi_reg_imm(BYTECODE_REGISTER_RSP, 5),
        movf_reg_imm(BYTECODE_REGISTER_RAX, a),
        addf_reg_imm(BYTECODE_REGISTER_RAX, b),
        subf_reg_imm(BYTECODE_REGISTER_RAX, c),
        mulf_reg_imm(BYTECODE_REGISTER_RAX, b),
        dec_reg(BYTECODE_REGISTER_RAX),
        movi_reg_imm(BYTECODE_REGISTER_R15, 12),
        store_local_reg(BYTECODE_REGISTER_RCX, BYTECODE_REGISTER_R15),
        load_local_reg(BYTECODE_REGISTER_R12, BYTECODE_REGISTER_RCX),
        nop(),
        nop(),
        store_local_imm(2, BYTECODE_REGISTER_R15),
        load_local_imm(BYTECODE_REGISTER_RCX, 2),
        xor_reg_reg(BYTECODE_REGISTER_RCX, BYTECODE_REGISTER_RCX),
        xor_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_RAX),
        movf_reg_imm(BYTECODE_REGISTER_RDI, a),
        movf_reg_imm(BYTECODE_REGISTER_RSI, b),
        call_imm(__add_floats),
        leave(),
        enter(),
        mov_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_RDI),
        add_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_RSI),
        lea_reg_imm(BYTECODE_REGISTER_RDI, 0),
        call_foreign(7, 14, 1, BYTECODE_VALUE_S32),
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

    if (write_bytecode_executable("./sample.bcr", program)) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
