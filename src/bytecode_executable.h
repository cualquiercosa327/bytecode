#ifndef BYTECODE_EXECUTABLE_H
#define BYTECODE_EXECUTABLE_H

#pragma pack(push, 1)
struct bytecode_header
{
    uint32_t stack_size;
    uint32_t data_size;
};

struct bytecode_executable
{
    struct bytecode_header header;
    char *data_segment;
    uint64_t *text_segment;
};
#pragma pack(pop)

#endif
