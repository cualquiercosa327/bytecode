### bytecode runner

registed-based virtual machine that can execute bytecode loaded from binary files.
there is no assembler yet, but programs can be written in a c-program
using easy-to-use macros and then have it output a bytecode binary file
that the virtual machine can execute (see provided samples).

supports multiple sized data-types (i8, i16, i32, i64, f32, f64)

uses a data segment for storing strings and global variables.

ffi support through dyncall (using c-calling convention).
