#ifndef EASM_TOKEN_TYPE_H
#define EASM_TOKEN_TYPE_H

#include <string>
#include <vector>

enum TokenType {
    IDENTIFIER,  // text, reg, pointer, section

    LITERAL,

    OPERATION,  // mov
    COMMA,      // x, y
    COLON,      // main:

    NEWLINE,
};

const std::vector<std::string> TokenTypeLookup{
    "IDENTIFIER", "LITERAL", "OPERATION", "COMMA", "COLON", "NEWLINE"};

std::string lookupToken(int type);

#endif