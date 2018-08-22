#include "bytecode_instruction_handler.h"
#include "bytecode_runner.h"
#include "bytecode_value.h"

#include <dlfcn.h>
#include <dyncall.h>
#include <assert.h>

static bytecode_instruction_handler *instruction_handlers[BYTECODE_OPCODE_COUNT] = {
    [BYTECODE_OPCODE_HALT]              = exec_op_halt,

    [BYTECODE_OPCODE_MOVU_REG_IMM]      = exec_op_movu_reg_imm,
    [BYTECODE_OPCODE_MOVI_REG_IMM]      = exec_op_movi_reg_imm,
    [BYTECODE_OPCODE_MOVF_REG_IMM]      = exec_op_movf_reg_imm,
    [BYTECODE_OPCODE_MOV_REG_REG]       = exec_op_mov_reg_reg,

    [BYTECODE_OPCODE_PUSHU_IMM]         = exec_op_pushu_imm,
    [BYTECODE_OPCODE_PUSHI_IMM]         = exec_op_pushi_imm,
    [BYTECODE_OPCODE_PUSHF_IMM]         = exec_op_pushf_imm,
    [BYTECODE_OPCODE_PUSH_REG]          = exec_op_push_reg,
    [BYTECODE_OPCODE_POP_REG]           = exec_op_pop_reg,

    [BYTECODE_OPCODE_ADDU_REG_IMM]      = exec_op_addu_reg_imm,
    [BYTECODE_OPCODE_ADDI_REG_IMM]      = exec_op_addi_reg_imm,
    [BYTECODE_OPCODE_ADDF_REG_IMM]      = exec_op_addf_reg_imm,
    [BYTECODE_OPCODE_ADD_REG_REG]       = exec_op_add_reg_reg,

    [BYTECODE_OPCODE_SUBU_REG_IMM]      = exec_op_subu_reg_imm,
    [BYTECODE_OPCODE_SUBI_REG_IMM]      = exec_op_subi_reg_imm,
    [BYTECODE_OPCODE_SUBF_REG_IMM]      = exec_op_subf_reg_imm,
    [BYTECODE_OPCODE_SUB_REG_REG]       = exec_op_sub_reg_reg,

    [BYTECODE_OPCODE_MULU_REG_IMM]      = exec_op_mulu_reg_imm,
    [BYTECODE_OPCODE_MULI_REG_IMM]      = exec_op_muli_reg_imm,
    [BYTECODE_OPCODE_MULF_REG_IMM]      = exec_op_mulf_reg_imm,
    [BYTECODE_OPCODE_MUL_REG_REG]       = exec_op_mul_reg_reg,

    [BYTECODE_OPCODE_NEG_REG]           = exec_op_neg_reg,
    [BYTECODE_OPCODE_INC_REG]           = exec_op_inc_reg,
    [BYTECODE_OPCODE_DEC_REG]           = exec_op_dec_reg,
    [BYTECODE_OPCODE_XOR_REG_REG]       = exec_op_xor_reg_reg,

    [BYTECODE_OPCODE_CALL_IMM]          = exec_op_call_imm,
    [BYTECODE_OPCODE_CALL_REG]          = exec_op_call_reg,
    [BYTECODE_OPCODE_CALL_FOREIGN]      = exec_op_call_foreign,

    [BYTECODE_OPCODE_ENTER]             = exec_op_enter,
    [BYTECODE_OPCODE_LEAVE]             = exec_op_leave,
    [BYTECODE_OPCODE_STORE_LOCAL_IMM]   = exec_op_store_local_imm,
    [BYTECODE_OPCODE_STORE_LOCAL_REG]   = exec_op_store_local_reg,
    [BYTECODE_OPCODE_LOAD_LOCAL_IMM]    = exec_op_load_local_imm,
    [BYTECODE_OPCODE_LOAD_LOCAL_REG]    = exec_op_load_local_reg,
    [BYTECODE_OPCODE_LEA_REG_IMM]       = exec_op_lea_reg_imm,
    [BYTECODE_OPCODE_LEA_REG_REG]       = exec_op_lea_reg_reg,

    [BYTECODE_OPCODE_NOP]               = exec_op_nop
};

void bytecode_instruction_execute(struct bytecode_runner *bcr, struct bytecode_instruction instr)
{
    if (instruction_handlers[instr.op]) {
        instruction_handlers[instr.op](bcr, instr.op, instr.r1, instr.r2);
    } else {
        fprintf(stderr, "%2X: missing instruction handler\n", instr.op);
    }
}

bytecode_instruction_handler_(exec_op_halt)
{
    bcr->is_running = false;
}

bytecode_instruction_handler_(exec_op_nop)
{
}

bytecode_instruction_handler_(exec_op_movu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bcr->reg[reg1] = u64_constant;
}

bytecode_instruction_handler_(exec_op_movi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bcr->reg[reg1] = s64_constant;
}

bytecode_instruction_handler_(exec_op_movf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bcr->reg[reg1] = f64_constant;
}

bytecode_instruction_handler_(exec_op_mov_reg_reg)
{
    bcr->reg[reg1] = bcr->reg[reg2];
}

bytecode_instruction_handler_(exec_op_pushu_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_runner_push_stack(bcr, u64_constant);
}

bytecode_instruction_handler_(exec_op_pushi_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_runner_push_stack(bcr, s64_constant);
}

bytecode_instruction_handler_(exec_op_pushf_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_runner_push_stack(bcr, f64_constant);
}

bytecode_instruction_handler_(exec_op_push_reg)
{
    bytecode_runner_push_stack(bcr, bcr->reg[reg1]);
}

bytecode_instruction_handler_(exec_op_pop_reg)
{
    bcr->reg[reg1] = bytecode_runner_pop_stack(bcr);
}

bytecode_instruction_handler_(exec_op_addu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_value_add(&bcr->reg[reg1], u64_constant);
}

bytecode_instruction_handler_(exec_op_addi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_value_add(&bcr->reg[reg1], s64_constant);
}

bytecode_instruction_handler_(exec_op_addf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_value_add(&bcr->reg[reg1], f64_constant);
}

bytecode_instruction_handler_(exec_op_add_reg_reg)
{
    bytecode_value_add(&bcr->reg[reg1], bcr->reg[reg2]);
}

bytecode_instruction_handler_(exec_op_subu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_value_sub(&bcr->reg[reg1], u64_constant);
}

bytecode_instruction_handler_(exec_op_subi_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_value_sub(&bcr->reg[reg1], s64_constant);
}

bytecode_instruction_handler_(exec_op_subf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_value_sub(&bcr->reg[reg1], f64_constant);
}

bytecode_instruction_handler_(exec_op_sub_reg_reg)
{
    bytecode_value_sub(&bcr->reg[reg1], bcr->reg[reg2]);
}

bytecode_instruction_handler_(exec_op_mulu_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_value_mul(&bcr->reg[reg1], u64_constant);
}

bytecode_instruction_handler_(exec_op_muli_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value s64_constant = bytecode_value_create_s64(constant);
    bytecode_value_mul(&bcr->reg[reg1], s64_constant);
}

bytecode_instruction_handler_(exec_op_mulf_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value f64_constant = bytecode_value_create_f64(constant);
    bytecode_value_mul(&bcr->reg[reg1], f64_constant);
}

bytecode_instruction_handler_(exec_op_mul_reg_reg)
{
    bytecode_value_mul(&bcr->reg[reg1], bcr->reg[reg2]);
}

bytecode_instruction_handler_(exec_op_neg_reg)
{
    bytecode_value_neg(&bcr->reg[reg1]);
}

bytecode_instruction_handler_(exec_op_inc_reg)
{
    struct bytecode_value one = bytecode_value_constant_one(bcr->reg[reg1]);
    bytecode_value_add(&bcr->reg[reg1], one);
}

bytecode_instruction_handler_(exec_op_dec_reg)
{
    struct bytecode_value one = bytecode_value_constant_one(bcr->reg[reg1]);
    bytecode_value_sub(&bcr->reg[reg1], one);
}

bytecode_instruction_handler_(exec_op_xor_reg_reg)
{
    bcr->reg[reg1]._u64 ^= bcr->reg[reg2]._u64;
}

bytecode_instruction_handler_(exec_op_call_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bytecode_runner_push_stack(bcr, bcr->reg[BYTECODE_REGISTER_RIP]);
    bcr->reg[BYTECODE_REGISTER_RIP] = u64_constant;
}

bytecode_instruction_handler_(exec_op_call_reg)
{
    bytecode_runner_push_stack(bcr, bcr->reg[BYTECODE_REGISTER_RIP]);
    bcr->reg[BYTECODE_REGISTER_RIP] = bcr->reg[reg1];
}

bytecode_instruction_handler_(exec_op_call_foreign)
{
    uint64_t sym_constant = fetch_instruction(bcr);
    char *sym_name = bcr->data + sym_constant;
    assert(sym_name < bcr->data + bcr->data_size);

    uint64_t lib_constant = fetch_instruction(bcr);
    char *lib_name = bcr->data + lib_constant;
    assert(lib_name < bcr->data + bcr->data_size);

    uint64_t reg_arg_count = fetch_instruction(bcr);
    uint64_t ret_kind = fetch_instruction(bcr);

    void *handle = dlopen(lib_name, RTLD_LAZY);
    assert(handle);

    void *func = dlsym(handle, sym_name);
    assert(func);

    DCCallVM *vm = dcNewCallVM(4096);
    dcMode(vm, DC_CALL_C_DEFAULT);
    dcReset(vm);

    for (unsigned i = 0; i < reg_arg_count; ++i) {
        struct bytecode_value *reg = &bcr->reg[bytecode_call_registers[i]];
        switch (reg->kind) {
        case BYTECODE_VALUE_S64:
        case BYTECODE_VALUE_U64: {
            dcArgLongLong(vm, reg->_s64);
        } break;
        case BYTECODE_VALUE_S32:
        case BYTECODE_VALUE_U32: {
            dcArgInt(vm, reg->_s64);
        } break;
        case BYTECODE_VALUE_S16:
        case BYTECODE_VALUE_U16: {
            dcArgShort(vm, reg->_s16);
        } break;
        case BYTECODE_VALUE_F64: {
            dcArgDouble(vm, reg->_f64);
        } break;
        case BYTECODE_VALUE_F32: {
            dcArgFloat(vm, reg->_f64);
        } break;
        case BYTECODE_VALUE_S8:
        case BYTECODE_VALUE_U8: {
            dcArgChar(vm, reg->_s8);
        } break;
        case BYTECODE_VALUE_POINTER: {
            dcArgPointer(vm, reg->ptr);
        } break;
        default: {
        } break;
        }
    }

    switch (ret_kind) {
    case BYTECODE_VALUE_S64:
    case BYTECODE_VALUE_U64: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_s64(dcCallLongLong(vm, func));
    } break;
    case BYTECODE_VALUE_S32:
    case BYTECODE_VALUE_U32: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_s32(dcCallInt(vm, func));
    } break;
    case BYTECODE_VALUE_S16:
    case BYTECODE_VALUE_U16: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_s16(dcCallShort(vm, func));
    } break;
    case BYTECODE_VALUE_S8:
    case BYTECODE_VALUE_U8: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_s8(dcCallChar(vm, func));
    } break;
    case BYTECODE_VALUE_F64: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_f64(dcCallDouble(vm, func));
    } break;
    case BYTECODE_VALUE_F32: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_f32(dcCallFloat(vm, func));
    } break;
    case BYTECODE_VALUE_POINTER: {
        bcr->reg[BYTECODE_REGISTER_RAX] = bytecode_value_create_ptr((uint64_t)dcCallPointer(vm, func));
    } break;
    default: {
        dcCallVoid(vm, func);
    } break;
    }

    dcFree(vm);
    dlclose(handle);
}

bytecode_instruction_handler_(exec_op_enter)
{
    bytecode_runner_push_stack(bcr, bcr->reg[BYTECODE_REGISTER_RBP]);
    bcr->reg[BYTECODE_REGISTER_RBP] = bcr->reg[BYTECODE_REGISTER_RSP];
}

bytecode_instruction_handler_(exec_op_leave)
{
    bcr->reg[BYTECODE_REGISTER_RSP] = bcr->reg[BYTECODE_REGISTER_RBP];
    bcr->reg[BYTECODE_REGISTER_RBP] = bytecode_runner_pop_stack(bcr);
    bcr->reg[BYTECODE_REGISTER_RIP] = bytecode_runner_pop_stack(bcr);
}

bytecode_instruction_handler_(exec_op_store_local_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + u64_constant._u64] = bcr->reg[reg1];
}

bytecode_instruction_handler_(exec_op_store_local_reg)
{
    bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + bcr->reg[reg1]._u64] = bcr->reg[reg2];
}

bytecode_instruction_handler_(exec_op_load_local_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    bcr->reg[reg1] = bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + u64_constant._u64];
}

bytecode_instruction_handler_(exec_op_load_local_reg)
{
    bcr->reg[reg1] = bcr->stack[bcr->reg[BYTECODE_REGISTER_RBP]._u64 + bcr->reg[reg2]._u64];
}

bytecode_instruction_handler_(exec_op_lea_reg_imm)
{
    uint64_t constant = fetch_instruction(bcr);
    struct bytecode_value u64_constant = bytecode_value_create_u64(constant);
    char *effective_address = bcr->data + u64_constant._u64;
    assert(effective_address < bcr->data + bcr->data_size);
    bcr->reg[reg1] = bytecode_value_create_ptr((uint64_t)effective_address);
}

bytecode_instruction_handler_(exec_op_lea_reg_reg)
{
    bcr->reg[reg1] = bytecode_value_create_ptr(bcr->reg[reg2]._u64);
}
