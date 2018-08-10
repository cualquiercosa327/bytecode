#ifndef BYTECODE_VALUE_H
#define BYTECODE_VALUE_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define LIST_OF_VALUE_KINDS \
    X(BYTECODE_VALUE_S64),\
    X(BYTECODE_VALUE_S32),\
    X(BYTECODE_VALUE_S16),\
    X(BYTECODE_VALUE_S8),\
    X(BYTECODE_VALUE_U64),\
    X(BYTECODE_VALUE_U32),\
    X(BYTECODE_VALUE_U16),\
    X(BYTECODE_VALUE_U8),\
    X(BYTECODE_VALUE_F64),\
    X(BYTECODE_VALUE_F32),\
    X(BYTECODE_VALUE_POINTER),\
    X(BYTECODE_VALUE_COUNT)

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

struct bytecode_value create_s64_constant(uint64_t s64);
struct bytecode_value create_s32_constant(uint64_t s32);
struct bytecode_value create_s16_constant(uint64_t s16);
struct bytecode_value create_s8_constant(uint64_t s8);

struct bytecode_value create_u64_constant(uint64_t u64);
struct bytecode_value create_u32_constant(uint64_t u32);
struct bytecode_value create_u16_constant(uint64_t u16);
struct bytecode_value create_u8_constant(uint64_t u8);

struct bytecode_value create_f64_constant(uint64_t f64);
struct bytecode_value create_f32_constant(uint64_t f32);

struct bytecode_value create_ptr_constant(uint64_t ptr);

void print_bytecode_value(struct bytecode_value *value, FILE *stream);

#endif
