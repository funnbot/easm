#ifndef EASVM_OPCODE_H
#define EASVM_OPCODE_H

#include <vector>
#include <string>

enum OpCode {
    OP_NOOP,
    OP_MOV,
    OP_PUSH, OP_POP,
    OP_LDR, OP_STR,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_INC, OP_DEC,
    OP_AND, OP_OR, OP_XOR,
    OP_JMP, OP_JNZ, OP_JZ,
    OP_CMP,
    OP_CALL, OP_SYSCALL,
    OP_RET,
    OP_EOF
};

const std::vector<std::string> OpCodeLookup { 
    "noop",
    "mov",
    "push", "pop",
    "ldr", "str",
    "add", "sub", "mul", "div", "mod",
    "inc", "dec",
    "and", "or", "xor",
    "jmp", "jnz", "jz",
    "cmp",
    "call", "syscall",
    "ret"
    "eof"  
};

std::string lookupName(int idx);

#endif