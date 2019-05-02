#ifndef EASM_PARSER_H
#define EASM_PARSER_H

#include <string>
#include <vector>
#include "statement.h"
#include "token.h"

class Parser {
   public:
    Parser(std::vector<Token>& tokens);
    ~Parser();
    std::vector<Statement*>& parseTokens();

   private:
    std::vector<Statement*> statements {};
    std::vector<Token>& tokens;
    int index = 0, line = 1;
    Token& current;

    void parseToken();

    Token& advance();
    Token& peek();
    bool match(TokenType type);
    bool consume(TokenType type, const std::string& error);
    bool eof();

    bool parseRegister(const std::string& str, RegSize& regSize, int& index);

    template<typename T>
    void hello(T v);
};

#endif