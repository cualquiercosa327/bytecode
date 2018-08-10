#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#define LIST_OF_VALUE_KINDS \
    X(Bytecode_Value_S64),\
    X(Bytecode_Value_S32),\
    X(Bytecode_Value_S16),\
    X(Bytecode_Value_S8),\
    X(Bytecode_Value_U64),\
    X(Bytecode_Value_U32),\
    X(Bytecode_Value_U16),\
    X(Bytecode_Value_U8),\
    X(Bytecode_Value_Float64),\
    X(Bytecode_Value_Float32),\
    X(Bytecode_Value_Pointer),\
    X(Bytecode_Value_Count)

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
    result.kind = Bytecode_Value_S64;
    result._s64 = (int64_t)(*(int64_t *)&s64);
    return result;
}

struct bytecode_value create_s32_constant(uint64_t s32)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_S32;
    result._s32 = (int32_t)(*(int32_t *)&s32);
    return result;
}

struct bytecode_value create_s16_constant(uint64_t s16)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_S16;
    result._s16 = (int16_t)(*(int16_t *)&s16);
    return result;
}

struct bytecode_value create_s8_constant(uint64_t s8)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_S8;
    result._s8 = (int8_t)(*(int8_t *)&s8);
    return result;
}

struct bytecode_value create_u64_constant(uint64_t u64)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_U64;
    result._u64 = (uint64_t)(*(uint64_t *)&u64);
    return result;
}

struct bytecode_value create_u32_constant(uint64_t u32)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_U32;
    result._u32 = (uint32_t)(*(uint32_t *)&u32);
    return result;
}

struct bytecode_value create_u16_constant(uint64_t u16)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_U16;
    result._u16 = (uint16_t)(*(uint16_t *)&u16);
    return result;
}

struct bytecode_value create_u8_constant(uint64_t u8)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_U8;
    result._u8 = (uint8_t)(*(uint8_t *)&u8);
    return result;
}

struct bytecode_value create_f64_constant(uint64_t f64)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_Float64;
    result._f64 = (double)(*(double *)&f64);
    return result;
}

struct bytecode_value create_f32_constant(uint64_t f32)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_Float32;
    result._f32 = (float)(*(float *)&f32);
    return result;
}

struct bytecode_value create_ptr_constant(uint64_t ptr)
{
    struct bytecode_value result;
    result.kind = Bytecode_Value_Pointer;
    result.ptr = (void *)ptr;
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
    BYTECODE_REGISTER_RIP,

    BYTECODE_REGISTER_RAX,
    BYTECODE_REGISTER_RBX,
    BYTECODE_REGISTER_RCX,
    BYTECODE_REGISTER_RDX,
    BYTECODE_REGISTER_RBP,
    BYTECODE_REGISTER_RSP,
    BYTECODE_REGISTER_RSI,
    BYTECODE_REGISTER_RDI,

    BYTECODE_REGISTER_R8,
    BYTECODE_REGISTER_R9,
    BYTECODE_REGISTER_R10,
    BYTECODE_REGISTER_R11,
    BYTECODE_REGISTER_R12,
    BYTECODE_REGISTER_R13,
    BYTECODE_REGISTER_R14,
    BYTECODE_REGISTER_R15,

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
    uint64_t cycle_count;

    uint64_t *text;
    uint32_t stack_head;
    uint32_t stack_size;
    struct bytecode_value *stack;

    struct bytecode_value reg[BYTECODE_REGISTER_COUNT];
};

void bytecode_runner_init(struct bytecode_runner *bcr, uint64_t *program)
{
    bcr->stack_head = 0;
    bcr->stack_size = 200;
    bcr->stack = malloc(bcr->stack_size * sizeof(struct bytecode_value));

    bcr->reg[BYTECODE_REGISTER_RIP] = create_u64_constant(0);

    bcr->text = program;
    bcr->cycle_count = 0;
    bcr->is_running = true;
}

void bytecode_runner_destroy(struct bytecode_runner *bcr)
{
    free(bcr->stack);
    memset(bcr, 0, sizeof(struct bytecode_runner));
}

struct bytecode_value bytecode_runner_result(struct bytecode_runner *bcr)
{
    return bcr->reg[BYTECODE_REGISTER_RAX];
}

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
    uint64_t raw_instr = fetch_instruction(bcr);
    struct bytecode_instruction instr = decode_instruction(raw_instr);

    printf("cycle %3" PRIu64 ": op = %2X, r1 = %2d, r2 = %2d\n",
            ++bcr->cycle_count, instr.op, instr.r1, instr.r2);

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
    printf("head = %" PRId64 "\n", ival._s64);

    bytecode_runner_destroy(&bcr);

    return 0;
}
