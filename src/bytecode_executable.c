#include "bytecode_executable.h"

#include <stdio.h>
#include <stdlib.h>

bool write_bytecode_executable(const char *absolutepath, struct bytecode_executable exe)
{
    FILE *handle = fopen(absolutepath, "wb");
    if (!handle) return false;

    fwrite(&exe.header, sizeof(struct bytecode_header), 1, handle);
    fwrite(exe.data_segment, exe.header.data_size, 1, handle);
    fwrite(exe.text_segment, exe.header.text_size, 1, handle);

    fclose(handle);
    return true;
}

bool load_bytecode_executable(const char *absolutepath, struct bytecode_executable *exe)
{
    FILE *handle = fopen(absolutepath, "rb");
    if (!handle) return false;

    fseek(handle, 0, SEEK_END);
    unsigned length = ftell(handle);
    fseek(handle, 0, SEEK_SET);

    char *contents = malloc(length + 1);
    fread(contents, length, 1, handle);
    contents[length] = '\0';

    struct bytecode_header *header = (struct bytecode_header *) contents;

    exe->header = *header;
    exe->data_segment = contents + sizeof(struct bytecode_header);
    exe->text_segment = (uint64_t *)((char *)exe->data_segment + exe->header.data_size);

    return true;
}
