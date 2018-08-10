#include "bytecode_value.h"

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
