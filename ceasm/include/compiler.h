#ifndef EASM_COMPILER_H
#define EASM_COMPILER_H

#include <map>
#include <vector>
#include "opcode.h"
#include "parameter.h"
#include "statement.h"

class Compiler {
   public:
    Compiler(std::vector<Statement*>& statements);

    std::vector<uint64_t>& compileBytecode();

   private:
    std::vector<Statement*>& statements;

    std::vector<uint64_t> bytes{};

    std::map<int, std::string> placeholders{};
    std::map<std::string, int> labels{};

    int index = 0, opdx = 0;

    void compileNext();
    void nextOp(OpStmt& stmt);
    void nextLabel(LabelStmt& stmt);

    void set8(uint64_t& b, uint8_t v, uint8_t i);
    void set16(uint64_t& b, uint16_t v, uint8_t i);
    void set32(uint64_t& b, uint32_t v, uint8_t i);

    Statement& advance();
    bool eof();
};

#endif