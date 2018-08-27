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
        "/usr/lib/libc.dylib\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0"
    };

    /*
     * uint64_t mem[40];
     * fib :: (n: int) -> int {
     *     if n <= 1 return n;
     *
     *     if mem[n - 1] == 0 {
     *         mem[n - 1] = fib(n - 1);
     *     }
     *
     *     if mem[n - 2] == 0 {
     *         mem[n - 2] = fib(n - 2);
     *     }
     *
     *     return mem[n - 1] + mem[n - 2];
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
        mov_i64_reg_imm(BYTECODE_REGISTER_R10, 1),
        cmp_reg_imm(BYTECODE_REGISTER_R10, 40),
        jg_imm(30),
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
        push_reg(BYTECODE_REGISTER_R8),
        push_reg(BYTECODE_REGISTER_R9),
        push_reg(BYTECODE_REGISTER_R10),
        push_reg(BYTECODE_REGISTER_R11),
        push_reg(BYTECODE_REGISTER_R12),
        push_reg(BYTECODE_REGISTER_R13),
        mov_reg_reg(BYTECODE_REGISTER_R8, BYTECODE_REGISTER_RDI),
        cmp_reg_imm(BYTECODE_REGISTER_R8, 1),
        jle_imm(80),
        mov_reg_reg(BYTECODE_REGISTER_R9, BYTECODE_REGISTER_R8),
        dec_reg(BYTECODE_REGISTER_R9),
        mov_reg_reg(BYTECODE_REGISTER_R10, BYTECODE_REGISTER_R9),
        dec_reg(BYTECODE_REGISTER_R10),
        lea_bss_reg_imm(BYTECODE_REGISTER_R11, 63),
        mov_reg_reg(BYTECODE_REGISTER_R12, BYTECODE_REGISTER_R11),
        mov_i64_reg_imm(BYTECODE_REGISTER_R13, 8),
        mul_reg_reg(BYTECODE_REGISTER_R13, BYTECODE_REGISTER_R9),
        add_reg_reg(BYTECODE_REGISTER_R11, BYTECODE_REGISTER_R13),
        mov_i64_reg_imm(BYTECODE_REGISTER_R13, 8),
        mul_reg_reg(BYTECODE_REGISTER_R13, BYTECODE_REGISTER_R10),
        add_reg_reg(BYTECODE_REGISTER_R12, BYTECODE_REGISTER_R13),
        memr_i64_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_R11),
        cmp_reg_imm(BYTECODE_REGISTER_RAX, 0),
        jnz_imm(68),
        mov_reg_reg(BYTECODE_REGISTER_RDI, BYTECODE_REGISTER_R9),
        call_imm(__fib),
        memw_reg_reg(BYTECODE_REGISTER_R11, BYTECODE_REGISTER_RAX),
        memr_i64_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_R12),
        cmp_reg_imm(BYTECODE_REGISTER_RAX, 0),
        jnz_imm(77),
        mov_reg_reg(BYTECODE_REGISTER_RDI, BYTECODE_REGISTER_R10),
        call_imm(__fib),
        memw_reg_reg(BYTECODE_REGISTER_R12, BYTECODE_REGISTER_RAX),
        memr_i64_reg_reg(BYTECODE_REGISTER_R9, BYTECODE_REGISTER_R12),
        memr_i64_reg_reg(BYTECODE_REGISTER_R8, BYTECODE_REGISTER_R11),
        add_reg_reg(BYTECODE_REGISTER_R8, BYTECODE_REGISTER_R9),
        mov_reg_reg(BYTECODE_REGISTER_RAX, BYTECODE_REGISTER_R8),
        pop_i64_reg(BYTECODE_REGISTER_R13),
        pop_i64_reg(BYTECODE_REGISTER_R12),
        pop_i64_reg(BYTECODE_REGISTER_R11),
        pop_i64_reg(BYTECODE_REGISTER_R10),
        pop_i64_reg(BYTECODE_REGISTER_R9),
        pop_i64_reg(BYTECODE_REGISTER_R8),
        end_call_frame(),
        ret()
    };

    struct bytecode_header program_header = {
        .magic = { 'b', 'c', 'r' },
        .abi_version = 0x1,
        .stack_size = 2048,
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
