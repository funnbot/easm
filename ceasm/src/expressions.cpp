#include <iostream>
#include "opcode.h"
#include "parameter.h"
#include "statement.h"
#include "tokentype.h"
using namespace std;

#define LINE(l) "[line " << l << "] "

void Statement::display(ostream& out) const {
    out << LINE(line);
}

void OpStmt::display(ostream& out) const {
    out << LINE(line) << "OP " << lookupName(op) << " ";
    if (params.size() > 0) {
        for (int i = 0; i < params.size() - 1; i++) out << *(params[i]) << ", ";
        out << *params.back();
    }
}

void LabelStmt::display(ostream& out) const {
    out << LINE(line) << name << ":";
}

void Parameter::display(ostream& out) const {}

const char rnames[] = {'b', 'w', 'd', 'r'};
void RegParam::display(ostream& out) const {
    out << rnames[static_cast<int>(type) - 1] << index + 1;
}

void LiteralParam::display(ostream& out) const {
    out << value;
}

void LabelParam::display(ostream& out) const {
    out << name;
}

int lookupCode(const std::string& op) {
    for (int i = 0; i < OpCodeLookup.size(); i++)
        if (op == OpCodeLookup[i]) return i;
    return -1;
}

std::string lookupName(int idx) { return OpCodeLookup[idx]; }
std::string lookupToken(int type) { return TokenTypeLookup[type]; }