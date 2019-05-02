#include "disassembler.h"
#include "parameter.h"
#include <sstream>
using namespace std;

Disassembler::Disassembler(vector<uint64_t>& bytes) 
    : bytes(bytes) {}

stringstream& Disassembler::disassemble() {
    uint64_t entry = advance();
    int entryPos = get32(entry, 1);
    source << "_main: " << entryPos << endl;

    while (!eof()) {
        nextByte();
    }

    return source;
}

void Disassembler::nextByte() {
    auto& current = advance();
    OpCode code = static_cast<OpCode>(get8(current, 7));
    source << index - 2 << ": " <<  lookupName(code);

    switch (code) {
        case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV: case OP_MOD:
        case OP_AND: case OP_OR: case OP_XOR:
        case OP_LDR: case OP_STR:
        case OP_MOV: case OP_CMP: {
            source << ' ';
            parseReg(get8(current, 5), get8(current, 4));
            source << ", ";
            if (get8(current, 6) == 0)
                parseReg(get8(current, 5), get8(current, 4));
            else
                source << get32(current, 0);
            break;
        }
        case OP_INC: case OP_DEC:
        case OP_PUSH: case OP_POP: {
            source << ' ';
            parseReg(get8(current, 5), get8(current, 4));
            break;
        }
        case OP_JMP: case OP_JZ: case OP_JNZ:
        case OP_CALL: {
            source << ' ' << get32(current, 0);
            break;
        }
        case OP_SYSCALL:
        case OP_RET: 
        case OP_NOOP:
        case OP_EOF: {
            break;
        }
    }
    source << endl;
}
const uint64_t& Disassembler::advance() {
    return bytes[index++];
}
bool Disassembler::eof() {
    return index >= bytes.size();
}

void Disassembler::parseReg(uint8_t sz, uint8_t idx) {
    switch (static_cast<RegSize>(sz)) {
        case RegSize::QWORD: source << "r"; break;
        case RegSize::DWORD: source << "d"; break;
        case RegSize::WORD: source << "w"; break;
        case RegSize::BYTE: source << "b"; break;
    }
    source << idx + 1;
}

uint8_t Disassembler::get8(uint64_t b, uint8_t i) {
    return (b >> i * 8) & 0xFF;
}
uint16_t Disassembler::get16(uint64_t b, uint8_t i) {
    return (b >> i * 16) & 0xFFFF;
}
uint32_t Disassembler::get32(uint64_t b, uint8_t i) {
    return (b >> i * 32) & 0xFFFFFFFF;
}