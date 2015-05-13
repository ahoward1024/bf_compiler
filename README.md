This is a brainfuck compiler.
The current compiler parses a .bf file for brainfuck characters, checks syntax, compiles a successful syntax check to a a C program (build.c), compiles the build.c with GCC and runs it.

There are future plans to compile the brainfuck code directly to x86 assembly.

Currently, in order to run the compiler GCC must be installed on your machine.