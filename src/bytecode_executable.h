#ifndef BYTECODE_EXECUTABLE_H
#define BYTECODE_EXECUTABLE_H

#include <stdbool.h>

#pragma pack(push, 1)
struct bytecode_header
{
    uint32_t stack_size;
    uint32_t data_size;
    uint64_t text_size;
};

struct bytecode_executable
{
    struct bytecode_header header;
    char *data_segment;
    uint64_t *text_segment;
};
#pragma pack(pop)

bool bytecode_write_executable(const char *absolutepath, struct bytecode_executable exe);
bool bytecode_load_executable(const char *absolutepath, struct bytecode_executable *exe);

#endif
