#ifndef EASM_LEXER_H
#define EASM_LEXER_H

#include <string>
#include <vector>
#include <istream>
#include "token.h"

class Lexer {
    std::vector<Token> tokens;
    std::istream& src;
    std::string lexeme;
    char current;
    int line = 1;

   public:
    Lexer(std::istream& src);
    std::vector<Token>& scanTokens();

   private:
    void scanToken();

    char advance();
    void addToken(TokenType type);

    void addString();
    void addIdentifier();
    void addChar();
    void skipLine();

    void addNumberLiteral();
    void addNumber();
    void addBinary();
    void addHex();

    char peek();
};

#endif