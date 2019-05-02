#include "lexer.h"
#include "opcode.h"
#include <istream>
using namespace std;

Lexer::Lexer(istream& src) : src(src) { }

vector<Token>& Lexer::scanTokens() {
    while (!src.eof()) {
        lexeme = "";
        scanToken();
    }

    tokens.push_back(Token(NEWLINE, "", line));
    return tokens;
}

void Lexer::scanToken() {
    char c = advance();
    switch(c) {
        case ',': addToken(COMMA); break;
        case ':': addToken(COLON); break;
        case ' ':
        case '\t':
        case '\r': break;
        case ';': skipLine(); break;
        case '\n': 
            lexeme = "";
            addToken(NEWLINE);
            line++;
            break;
        default: 
            if (isalpha(c) || c == '_') addIdentifier();
            else if (isdigit(c)) addNumber();
            break;
    }
}

char Lexer::advance() {
    current = src.get();
    lexeme += current;
    return current;
}

void Lexer::addToken(TokenType type) {
    Token token(type, lexeme, line);
    tokens.push_back(token);
    lexeme = "";
}

void Lexer::addIdentifier() {
    char pk;
    while ((pk = peek()) && (isalnum(pk) || pk == '_')) advance();
    if (lookupCode(lexeme) > -1)
        addToken(OPERATION);
    else addToken(IDENTIFIER);
}

void Lexer::addNumber() {
    char pk;
    while ((pk = peek()) && isdigit(pk)) advance();
    addToken(LITERAL);
}

void Lexer::skipLine() {
    char pk;
    while ((pk = peek()) && pk != '\n') src.get();
}

char Lexer::peek() {
    return src.eof() ? '\0' : src.peek();
}