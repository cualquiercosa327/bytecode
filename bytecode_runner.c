#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define LIST_OF_VALUE_KINDS \
    X(Value_S64),\
    X(Value_S32),\
    X(Value_S16),\
    X(Value_S8),\
    X(Value_U64),\
    X(Value_U32),\
    X(Value_U16),\
    X(Value_U8),\
    X(Value_Float64),\
    X(Value_Float32),\
    X(Value_Pointer),\
    X(Value_Count)

enum bytecode_value_kind
{
#define X(name) name
    LIST_OF_VALUE_KINDS
#undef X
};

static const char *bytecode_value_kind_str[] =
{
#define X(name) #name
    LIST_OF_VALUE_KINDS
#undef X
};

struct bytecode_value
{
    enum bytecode_value_kind kind;
    union {
        int64_t  _s64;
        int32_t  _s32;
        int16_t  _s16;
        int8_t   _s8;

        uint64_t _u64;
        uint32_t _u32;
        uint16_t _u16;
        uint8_t  _u8;

        double   _f64;
        float    _f32;

        void     *ptr;
    };
};

struct bytecode_value create_s64_constant(uint64_t s64)
{
    struct bytecode_value result;
    result.kind = Value_S64;
    result._s64 = (int64_t)(*(int64_t *)&s64);
    return result;
}

struct bytecode_value create_f32_constant(uint64_t f32)
{
    struct bytecode_value result;
    result.kind = Value_Float32;
    result._f32 = (float)(*(float *)&f32);
    return result;
}

enum bytecode_opcode
{
    HALT           = 0x00,

    PUSH_INT       = 0x10,
    PUSH_FLOAT     = 0x11,

    ADD_INT_IMM    = 0x20,
    ADD_FLOAT_IMM  = 0x30,
};

enum bytecode_register
{
    RIP,

    RAX, RBX, RCX, RDX, RBP, RSP, RSI, RDI,
    R8, R9, R10, R11, R12, R13, R14, R15,

    BYTECODE_REGISTER_COUNT
};

struct bytecode_instruction
{
    uint8_t op;
    uint8_t r1;
    uint8_t r2;
};

struct bytecode_runner
{
    bool is_running;

    uint64_t *text;
    uint32_t stack_head;
    uint32_t stack_size;
    struct bytecode_value *stack;

    uint64_t reg[BYTECODE_REGISTER_COUNT];
};

void bytecode_runner_init(struct bytecode_runner *bcr, uint64_t *program)
{
    bcr->stack_head = 0;
    bcr->stack_size = 200;
    bcr->stack = malloc(bcr->stack_size * sizeof(struct bytecode_value));

    bcr->text = program;
    bcr->is_running = true;
}

uint64_t fetch_instruction(struct bytecode_runner *bcr)
{
    return bcr->text[bcr->reg[RIP]++];
}

struct bytecode_instruction decode_instruction(uint64_t raw_instr)
{
    struct bytecode_instruction instr;
    instr.op = (raw_instr & 0xff00000000000000) >> 56;
    instr.r1 = (raw_instr & 0x00f0000000000000) >> 52;
    instr.r2 = (raw_instr & 0x000f000000000000) >> 48;
    return instr;
}

void push_stack(struct bytecode_runner *bcr, struct bytecode_value value)
{
    bcr->stack[bcr->stack_head++] = value;
    assert(bcr->stack_head < bcr->stack_size);
}

struct bytecode_value pop_stack(struct bytecode_runner *bcr)
{
    assert(bcr->stack_head > 0);
    return bcr->stack[--bcr->stack_head];
}

void do_cycle(struct bytecode_runner *bcr)
{
    static uint64_t cycle_count = 0;
    uint64_t raw_instr = fetch_instruction(bcr);
    struct bytecode_instruction instr = decode_instruction(raw_instr);

    printf("cycle %3ld: op = %2X, r1 = %2d, r2 = %2d\n",
            ++cycle_count, instr.op, instr.r1, instr.r2);

    switch (instr.op) {

    case HALT: {
        bcr->is_running = false;
    } break;

    case PUSH_INT: {
        uint64_t constant = fetch_instruction(bcr);
        struct bytecode_value s64_constant = create_s64_constant(constant);
        push_stack(bcr, s64_constant);
    } break;

    case PUSH_FLOAT: {
        uint64_t constant = fetch_instruction(bcr);
        struct bytecode_value f32_constant = create_f32_constant(constant);
        push_stack(bcr, f32_constant);
    } break;

    case ADD_INT_IMM: {
        struct bytecode_value rhs = pop_stack(bcr);
        struct bytecode_value lhs = pop_stack(bcr);
        assert(lhs.kind == rhs.kind);
        lhs._s64 += rhs._s64;
        push_stack(bcr, lhs);
    } break;

    case ADD_FLOAT_IMM: {
        struct bytecode_value rhs = pop_stack(bcr);
        struct bytecode_value lhs = pop_stack(bcr);
        assert(lhs.kind == rhs.kind);
        lhs._f32 += rhs._f32;
        push_stack(bcr, lhs);
    } break;

    }
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

int main(int argc, char **argv)
{
    float a = 22.3f;
    float b =  3.2f;

    int64_t c = 0x1234567890abcdef;
    int64_t d = 0x1234567890abcdef;

    uint64_t program[] = {
        encode_instruction(PUSH_INT), c,
        encode_instruction(PUSH_INT), d,
        encode_instruction(ADD_INT_IMM),
        encode_instruction(PUSH_FLOAT), (uint64_t)(*(uint64_t *)&a),
        encode_instruction(PUSH_FLOAT), (uint64_t)(*(uint64_t *)&b),
        encode_instruction(ADD_FLOAT_IMM),
        encode_instruction(HALT),
    };

    struct bytecode_runner bcr = {};
    bytecode_runner_init(&bcr, program);

    while (bcr.is_running) {
        do_cycle(&bcr);
    }

    struct bytecode_value fval = pop_stack(&bcr);
    printf("head = %.2f\n", fval._f32);

    struct bytecode_value ival = pop_stack(&bcr);
    printf("head = %ld\n", ival._s64);

    return 0;
}
