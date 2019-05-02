#include "parser.h"
#include "tokentype.h"
#include <sstream>
#include <iomanip>
using namespace std;

Parser::Parser(vector<Token>& tokens)
    : tokens(tokens), current(tokens[0]) {}

Parser::~Parser() {
    for (int i = 0; i < statements.size(); i++)
        delete statements[i];
}

vector<Statement*>& Parser::parseTokens() {
    while (!eof()) {
        parseToken();
    }

    return statements;
}

void Parser::parseToken() {
    advance();
    switch (current.type) {
        case NEWLINE: line++; break;
        case IDENTIFIER:
            if (match(COLON)) {
                statements.push_back(new LabelStmt(line, current.lexeme));
                advance();
            } else CEASM::error("Expect identifier to be label.", current.line);
            break;
        case OPERATION: {
            vector<Parameter*> params;
            OpCode code = static_cast<OpCode>(lookupCode(current.lexeme));

            do {
                advance();
                if (current.type == IDENTIFIER) {
                    RegSize regSize;
                    int idx;
                    if (parseRegister(current.lexeme, regSize, idx)) {
                        params.push_back(new RegParam(regSize, idx));
                    } else {
                        params.push_back(new LabelParam(current.lexeme));
                    }
                } else if (current.type == LITERAL) {
                    params.push_back(new LiteralParam(stoul(current.lexeme, nullptr)));
                } else if (current.type == NEWLINE) {
                    break;
                }
            } while (match(COMMA) && ++index);

            if (current.type == COMMA) {
                CEASM::error("Expect parameter after comma.", line);
                break;
            }

            statements.push_back(new OpStmt(line, code, params));
            break;
        }

        default: CEASM::error("Unexpected token.", line); break;
    }
}

Token& Parser::advance() {
    current = tokens[index++];
    return current;
}

Token& Parser::peek() {
    return tokens[index];
}

bool Parser::match(TokenType type) {
    return !eof() && peek().type == type;
}

bool Parser::consume(TokenType type, const string& error) {
    if (match(type)) {
        advance();
        return true;
    } else {
        CEASM::error(error, line);
        return false;
    }
}

bool Parser::eof() {
    return index >= tokens.size();
}

bool Parser::parseRegister(const string& str, RegSize& regSize, int& idx) {
    int strSize = str.size();
    if (strSize != 2 && strSize != 3) return false;

    switch (str[0]) {
        case 'r': regSize = RegSize::QWORD; break;
        case 'd': regSize = RegSize::DWORD; break; 
        case 'w': regSize = RegSize::WORD; break;
        case 'b': regSize = RegSize::BYTE; break;
        default: return false;
    }

    try { 
        idx = stoi(str.substr(1));
    }
    catch(std::exception e) {
        return false;
    }
    
    if (idx < 1 || idx > 16) return false;
    return true;
}