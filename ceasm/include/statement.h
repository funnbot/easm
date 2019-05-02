#ifndef EASM_STATEMENT_H
#define EASM_STATEMENT_H

#include <ostream>
#include <string>
#include <vector>
#include "opcode.h"
#include "parameter.h"

struct Statement {
    int line;

    Statement(int line) : line(line) {}
    friend std::ostream& operator<<(std::ostream& out, const Statement& instance);

   protected:
    virtual void display(std::ostream& out) const;
};

struct OpStmt : public Statement {
    OpCode op;
    std::vector<Parameter*> params;

    OpStmt(int line, OpCode op, std::vector<Parameter*> params)
        : Statement(line), op(op), params(params) {}

   protected:
    virtual void display(std::ostream& out) const;
};

struct LabelStmt : public Statement {
    std::string name;

    LabelStmt(int line, std::string name)
        : Statement(line), name(name) {}

   protected:
    virtual void display(std::ostream& out) const;
};

inline std::ostream& operator<<(std::ostream& out, const Statement& inst) {
    inst.display(out);
    return out;
}

#endif