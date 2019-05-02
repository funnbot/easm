#ifndef EASM_TOKEN_H
#define EASM_TOKEN_H

#include <iostream>
#include <ostream>
#include <string>
#include "tokentype.h"

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type, std::string lexeme, int line)
        : type(type), lexeme(lexeme), line(line){};

    friend std::ostream& operator<<(std::ostream& out, const Token& instance);
};

inline std::ostream& operator<<(std::ostream& out, const Token& inst) {
    out << "[line " << inst.line << "] " << lookupToken(inst.type) << " " << inst.lexeme;
    return out;
}

#endif