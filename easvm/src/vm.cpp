#include "vm.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "opcode.h"
using namespace std;

const size_t STACK_SIZE = 8;
const size_t HEAP_SIZE = 16;

// |7|6|5|4|3|2|1|0|
#define READ_BYTE(n, i) (n >> i * 8) & 0xFF
#define READ_OP(i) (op >> i * 8) & 0xFF
// |   1   |   0   |
#define READ_INT(n, i) (n >> i * 32) & 0xFFFFFFFF
#define SET_FLAGS_WITH(n) f_zero = n == 0

#define ARITH_REG() reg[READ_OP(P1_IDX)]
#define ARITH_VAL() (READ_OP(MODE) ? READ_INT(op, 0) : reg[READ_OP(P2_IDX)])
#define SET_FLAGS() SET_FLAGS_WITH(reg[READ_OP(P1_IDX)])

const size_t OP = 7, MODE = 6, P1_SIZE = 5, P1_IDX = 4, P2_SIZE = 3, P2_IDX = 2;

template<typename T>
void printArray(const T* arr, size_t size) {
    cout << '[';
    for (int i = 0; i < size - 1; i++) {
        cout << arr[i] << ", ";
    }
    cout << arr[size - 1] << ']' << endl;
}

int execute(const vector<uint64_t> bytes) {
    size_t length = bytes.size();
    // OP step
    size_t step = 0;
    // Header data
    uint64_t header = bytes[0];
    // Entry
    step = READ_INT(header, 1);
    // Flags
    bool f_zero = 0;
    // Registers
    uint64_t reg[16] {};
    // Memory
    size_t stdx = 0;
    uint64_t stack[STACK_SIZE];
    uint64_t heap[HEAP_SIZE];
    //uint8_t *const stack = new uint8_t[STACK_SIZE];
    //uint8_t *const heap = new uint8_t[HEAP_SIZE];

    while (step <= length) {
        const uint64_t& op = bytes[++step];
        OpCode code = static_cast<OpCode>(READ_BYTE(op, OP));
        //cout << lookupName(code) << endl;
        switch (code) {
            case OP_NOOP: break;
            case OP_MOV: ARITH_REG() = ARITH_VAL(); break;
            case OP_PUSH: stack[stdx++] = reg[READ_OP(P1_IDX)]; break;
            case OP_POP: reg[READ_OP(P1_IDX)] = stack[--stdx]; break;
            case OP_LDR: reg[READ_OP(P1_IDX)] = heap[reg[READ_OP(P2_IDX)]]; break;
            case OP_STR: heap[reg[READ_OP(P2_IDX)]] = reg[READ_OP(P1_IDX)]; break;
            case OP_ADD: ARITH_REG() += ARITH_VAL(); SET_FLAGS(); break;
            case OP_SUB: ARITH_REG() -= ARITH_VAL(); SET_FLAGS(); break;
            case OP_MUL: ARITH_REG() *= ARITH_VAL(); SET_FLAGS(); break;
            case OP_DIV: ARITH_REG() /= ARITH_VAL(); SET_FLAGS(); break;
            case OP_MOD: ARITH_REG() %= ARITH_VAL(); SET_FLAGS(); break;
            case OP_INC: reg[READ_OP(P1_IDX)]++; break;
            case OP_DEC: reg[READ_OP(P1_IDX)]--; break;
            case OP_AND: ARITH_REG() &= ARITH_VAL(); SET_FLAGS(); break;
            case OP_OR: ARITH_REG() |= ARITH_VAL(); SET_FLAGS(); break;
            case OP_XOR: ARITH_REG() ^= ARITH_VAL(); SET_FLAGS(); break;
            case OP_JMP: step = READ_INT(op, 0); break;
            case OP_JNZ: step = !f_zero ? READ_INT(op, 0) : step; break;
            case OP_JZ: step = f_zero ? READ_INT(op, 0) : step; break;
            case OP_CMP: f_zero = ARITH_REG() == ARITH_VAL(); break;
            case OP_SYSCALL: 
                switch (reg[0]) {
                    case 0: return reg[1]; break;
                    case 1: cout << static_cast<char>(reg[1]); break;
                    case 2: cout << reg[1]; break;
                    case 3: cin >> reg[1]; break;
                    default: cerr << "Invalid syscall code: " << reg[0] << endl; break;
                }
                break;
            case OP_CALL: break;
            case OP_RET: break;
            case OP_EOF: return 1;
            default: cerr << "Unknown code: " << code;
        }
    } 

    return 1;
}

bool openFile(const string& file, ifstream& ifile) {
    ifile.open(file);
    if (!ifile.is_open()) {
        error("Could not open file '" + file + "'");
        return false;
    }
    return true;
}

vector<uint64_t> readBytes(ifstream& ifile) {
    vector<uint64_t> bytes;
    while (!ifile.eof()) {
        uint64_t dat;
        ifile.read(reinterpret_cast<char*>(&dat), sizeof(dat));
        bytes.push_back(dat);
    }
    return bytes;
}

void error(const string& msg) {
    cerr << "Error: " << msg << endl;
    hadError = true;
}

string lookupName(int idx) { return OpCodeLookup[idx]; }