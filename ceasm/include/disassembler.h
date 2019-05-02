#ifndef EASM_DISASSEMBLER_H
#define EASM_DISASSEMBLER_H

#include <vector>
#include <string>
#include <sstream>
#include "opcode.h"

class Disassembler {
   public:
    Disassembler(std::vector<uint64_t>& bytes);
    std::stringstream& disassemble();

   private:
    std::vector<uint64_t>& bytes;
    std::stringstream source{};
    int index = 0;

    void nextByte();
    const uint64_t& advance();
    bool eof();

    void parseReg(uint8_t sz, uint8_t idx);

    uint8_t get8(uint64_t b, uint8_t i);
    uint16_t get16(uint64_t b, uint8_t i);
    uint32_t get32(uint64_t b, uint8_t i);
};

#endif