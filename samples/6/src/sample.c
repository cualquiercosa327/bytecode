#include "../../../src/bytecode_runner.h"
#include "../../../src/bytecode_opcode.h"

#include "../../../src/bytecode_instruction.h"
#include "../../../src/bytecode_instruction.c"

#include "../../../src/bytecode_executable.h"
#include "../../../src/bytecode_executable.c"

int main(int argc, char **argv)
{
    uint64_t __main = 4;
    uint64_t __fib = 32;


    char program_data[] = {
        "the %2d'th fibonacci number is %2d\n\0"
        "printf\0"
        "/usr/lib/libc.dylib"
    };

    /*
     * fib :: (n: int) -> int {
     *     if n <= 1 return n;
     *     return fib(n - 1) + fib(n - 2);
     * }

     * main :: () {
     *     for i := 1; i <= 10; i += 1 {
     *         printf("the %2d'th fibonacci number is %2d\n", i, fib(i));
     *     }
     * }
     */

    uint64_t program_text[] = {
        mov_i64_reg_imm(BYTECODE_REGISTER_RAX, __main),
        call_reg(BYTECODE_REGISTER_RAX),
        halt(),
        begin_call_frame(),
        mov_i8_reg_imm(BYTECODE_REGISTER_R10, 1),
        cmp_reg_imm(BYTECODE_REGISTER_R10, 11),
        jz_imm(30),
        mov_reg_reg(BYTECODE_REGISTER_RDI, BYTECODE_REGISTER_R10),
        call_imm(__fib),
        lea_bss_reg_imm(BYTECODE_REGISTER_RDI, 0),
        mov_reg_reg(BYTECODE_REGISTER_RSI, BYTECODE_REGISTER_R10),
        mov_reg_reg(BYTECODE_REGISTER_RDX, BYTECODE_REGISTER_RAX),
        lea_bss_reg_imm(BYTECODE_REGISTER_R11, 36),
        lea_bss_reg_imm(BYTECODE_REGISTER_R12, 43),
        push_reg(BYTECODE_REGISTER_R12),
        push_reg(BYTECODE_REGISTER_R11),
        call_foreign(3, BYTECODE_REGISTER_KIND_I32),
        inc_reg(BYTECODE_REGISTER_R10),
        jmp_imm(7),
        end_call_frame(),
        ret(),
        begin_call_frame(),
        push_reg(BYTECODE_REGISTER_R11),
        push_reg(BYTECODE_REGISTER_R12),
        mov_reg_reg(BYTECODE_REGISTER_R11, BYTECODE_REGISTER_RDI),
        cmp_reg_imm(BYTECODE_REGISTER_R11, 1),
        jle_imm(51),
        dec_reg(BYTECODE_REGISTER_R11),
        mov_reg_reg(BYTECODE_REGISTER_RDI, BYTECODE_REGISTER_R11),
        call_imm(__fib),
        mov_reg_reg(BYTECODE_REGISTER_R12, BYTECODE_REGISTER_RAX),
        dec_reg(BYTECODE_REGISTER_R11),
        mov_reg_reg(BYTECODE_REGISTER_RDI, BYTECODE_REGISTER_R11),
        call_imm(__fib),
        add_reg_reg(BYTECODE_REGISTER_R12, BYTECODE_REGISTER_RAX),
        mov_reg_reg(BYTECODE_REGISTER_R11, BYTECODE_REGISTER_R12),
        mov_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_R11),
        pop_i8_reg(BYTECODE_REGISTER_R12),
        pop_i8_reg(BYTECODE_REGISTER_R11),
        end_call_frame(),
        ret()
    };

    struct bytecode_header program_header = {
        .magic = { 'b', 'c', 'r' },
        .abi_version = 0x1,
        .stack_size = 100000,
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
