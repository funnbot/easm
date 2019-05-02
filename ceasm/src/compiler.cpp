#include "compiler.h"
#include <iostream>
using namespace std;

#define FOR_EACH(vector, i, elem) \
    for (int i = 0; i < vector.size(); i++) { \
        auto& elem = vector[i];

#define TRY_CAST(type, val, out) type* out = dynamic_cast<type*>(val)
#define ERROR_RET(msg, line) { CEASM::error(msg, line); return; }

Compiler::Compiler(vector<Statement*>& statements)
    : statements(statements) {}

vector<uint64_t>& Compiler::compileBytecode() {
    // Entry data, _main location and such
    bytes.push_back(0);

    while (!eof()) {
        compileNext();
    }

    if (!labels.count("_main")) CEASM::error("Expect entry label _main:");

    uint64_t eofByte = 0;
    set8(eofByte, OP_EOF, 7);
    bytes.push_back(eofByte);
    return bytes;
}

void Compiler::compileNext() {
    auto& current = advance();
    if (OpStmt* stmt = dynamic_cast<OpStmt*>(&current))
        nextOp(*stmt);
    else if (LabelStmt* stmt = dynamic_cast<LabelStmt*>(&current))
        nextLabel(*stmt);
}

//  |7|6|5|4|3|2|1|0|
//  | 3 | 2 | 1 | 0 |
//  |   1   |   0   |

void Compiler::nextOp(OpStmt& stmt) {
    uint64_t dat = 0;
    set8(dat, stmt.op, 7);
    switch (stmt.op) {
        case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV: case OP_MOD:
        case OP_AND: case OP_OR: case OP_XOR:
        case OP_LDR: case OP_STR:
        case OP_MOV: case OP_CMP: {
            if (stmt.params.size() != 2) ERROR_RET("Takes 2 parameters", stmt.line)
            FOR_EACH(stmt.params, i, current)
                if (TRY_CAST(RegParam, current, p)) {
                    set8(dat, static_cast<uint8_t>(p->type), 5 - (2 * i));
                    set8(dat, p->index - 1, 4 - (2 * i));
                } else if (TRY_CAST(LiteralParam, current, p)) {
                    if (i == 0) ERROR_RET("Cannot move into a literal.", stmt.line)
                    set8(dat, 0x0001, 6);
                    set32(dat, p->value, 0);
                } else ERROR_RET("Label parameter.", stmt.line)
            }
            break;
        }
        case OP_INC: case OP_DEC:
        case OP_PUSH: case OP_POP: {
            if (stmt.params.size() != 1) ERROR_RET("Takes 1 paramter", stmt.line)
            auto& current = stmt.params[0];
            if (TRY_CAST(RegParam, current, p)) {
                set8(dat, static_cast<uint8_t>(p->type), 5);
                set8(dat, p->index - 1, 4);
            } else ERROR_RET("Expected a register parameter.", stmt.line)
            break;
        }
        case OP_JMP: case OP_JZ: case OP_JNZ:
        case OP_CALL: {
            if (stmt.params.size() != 1) ERROR_RET("Takes 1 parameter", stmt.line)
            auto& current = stmt.params[0];
            if (TRY_CAST(LabelParam, current, p)) {
                if (labels.count(p->name)) set32(dat, labels[p->name], 0);
                else placeholders[opdx + 1] = p->name;  
            }
            break;
        }
        case OP_SYSCALL:
        case OP_RET: 
        case OP_NOOP: {
            if (stmt.params.size() != 0) ERROR_RET("Takes 0 parameters", stmt.line);
            break;
        }
        case OP_EOF: ERROR_RET("EOF is not allowed.", stmt.line);
    }
    bytes.push_back(dat);
    opdx++;
}

void Compiler::nextLabel(LabelStmt& stmt) {
    if (labels.count(stmt.name)) ERROR_RET("Duplicate label.", stmt.line)
    labels[stmt.name] = opdx;
    
    for (auto& kv : placeholders) {
        if (kv.second == stmt.name) 
            set32(bytes[kv.first], opdx, 0);
    }

    if (stmt.name == "_main") {
        set32(bytes[0], opdx, 1);
    }
}

void Compiler::set8(uint64_t& b, uint8_t v, uint8_t i) {
    b = (b & ~(0xFFULL << (i * 8))) | static_cast<uint64_t>(v) << (i * 8);
}
void Compiler::set16(uint64_t& b, uint16_t v, uint8_t i) {
    b = (b & ~(0xFFFFULL << (i * 16))) | static_cast<uint64_t>(v) << (i * 16);
}
void Compiler::set32(uint64_t& b, uint32_t v, uint8_t i) {
    b = (b & ~(0xFFFFFFFFULL << (i * 32))) | static_cast<uint64_t>(v) << (i * 32);
}

Statement& Compiler::advance() {
    return *statements[index++];
}

bool Compiler::eof() {
    return index >= statements.size();
}