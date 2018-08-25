#include "../../../src/bytecode_runner.h"
#include "../../../src/bytecode_opcode.h"

#include "../../../src/bytecode_instruction.h"
#include "../../../src/bytecode_instruction.c"

#include "../../../src/bytecode_executable.h"
#include "../../../src/bytecode_executable.c"

int main(int argc, char **argv)
{
    uint64_t __main = 4;
    uint64_t __fact = 26;


    char program_data[] = {
        '%','d',' ','f','a','c','t','o','r','i','a','l',' ','i','s',' ','%','d','\n','\0',
        'p','r','i','n','t','f','\0',
        '/','u','s','r','/','l','i','b','/','l','i','b','c','.','d','y','l','i','b','\0'
    };

    /*
     * factorial :: (n: int) -> int {
     *     if n == 1 return 1;
     *     return n * factorial(n - 1);
     * }
     * main :: () -> int {
     *     int num = 5;
     *     return printf("%d factorial is %d\n", num, factorial(num));
     * }
     */

    uint64_t program_text[] = {
        mov_i64_reg_imm(BYTECODE_REGISTER_RAX, __main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        begin_call_frame(),
        mov_i8_reg_imm(BYTECODE_REGISTER_R15, 5),
        mov_reg_reg(BYTECODE_REGISTER_RDI, BYTECODE_REGISTER_R15),
        mov_i64_reg_imm(BYTECODE_REGISTER_RAX, __fact),
        call_reg(BYTECODE_REGISTER_RAX),
        lea_bss_reg_imm(BYTECODE_REGISTER_RDI, 0),
        mov_reg_reg(BYTECODE_REGISTER_RDX, BYTECODE_REGISTER_RAX),
        mov_reg_reg(BYTECODE_REGISTER_RSI, BYTECODE_REGISTER_R15),
        lea_bss_reg_imm(BYTECODE_REGISTER_R11, 20),
        lea_bss_reg_imm(BYTECODE_REGISTER_R12, 27),
        push_reg(BYTECODE_REGISTER_R12),
        push_reg(BYTECODE_REGISTER_R11),
        call_foreign(3, BYTECODE_REGISTER_KIND_I32),
        end_call_frame(),
        ret(),
        begin_call_frame(),
        push_reg(BYTECODE_REGISTER_R10),
        mov_reg_reg(BYTECODE_REGISTER_R10, BYTECODE_REGISTER_RDI),
        cmp_reg_imm(BYTECODE_REGISTER_R10, 1),
        jnz_imm(38),
        sub_i8_reg_imm(BYTECODE_REGISTER_RDI, 1),
        call_imm(__fact),
        mul_reg_reg(BYTECODE_REGISTER_R10, BYTECODE_REGISTER_RAX),
        mov_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_R10),
        pop_i8_reg(BYTECODE_REGISTER_R10),
        end_call_frame(),
        ret()
    };

    struct bytecode_header program_header = {
        .magic = { 'b', 'c', 'r' },
        .abi_version = 0x1,
        .stack_size = 1024,
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
