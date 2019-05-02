#include "ceasm.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "compiler.h"
#include "disassembler.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
using namespace std;

bool CEASM::hadError = false;

vector<uint64_t> CEASM::compile(istream& src) {
    Lexer lexer(src);
    vector<Token>& tokens = lexer.scanTokens();
    if (hadError) return {};

    Parser parser(tokens);
    vector<Statement*>& statements = parser.parseTokens();
    if (hadError) return {};

    Compiler compiler(statements);
    vector<uint64_t>& bytes = compiler.compileBytecode();
    if (hadError) return {};

    return bytes;
}

void CEASM::compileDebug(istream& src) {
    Lexer lexer(src);
    vector<Token>& tokens = lexer.scanTokens();

    cout << "\nTokens:\n";
    for (auto& tok : tokens) {
        cout << tok << endl;
    }
    if (hadError) return;

    Parser parser(tokens);
    vector<Statement*>& statements = parser.parseTokens();

    cout << "\nStatements:\n";
    for (auto& stmt : statements) {
        cout << *stmt << endl;
    }
    if (hadError) return;

    Compiler compiler(statements);
    vector<uint64_t>& bytes = compiler.compileBytecode();

    cout << "\nBytecode:\n";

    for (auto& byt : bytes) {
        for (int i = 63; i >= 0; i--) {
            cout << bitset<64>(byt)[i];
            if (i % 8 == 0) cout << " ";
        }
        cout << endl;
    }
    if (hadError) return;

    Disassembler disassembler(bytes);
    stringstream& source = disassembler.disassemble();

    cout << "\nDisassembled:\n"
         << source.str();
}

bool CEASM::openFile(const string& file, ifstream& ifile) {
    ifile.open(file);
    if (!ifile.is_open()) {
        error("Could not open file '" + file + "'");
        return false;
    } else
        return true;
}

bool CEASM::writeFile(vector<uint64_t>& bytes, const string& file) {
    ofstream ofile(file + ".b");
    if (!ofile.is_open()) {
        error("Could not write file '" + file + "'");
        return false;
    }
    for (auto& b : bytes) {
        ofile.write(reinterpret_cast<char*>(&b), sizeof(b));
    }
    ofile.close();
    return true;
}

void CEASM::error(const string& msg) {
    cerr << "Error: " << msg << endl;
    hadError = true;
}
void CEASM::error(const string& msg, int line) {
    cerr << "[line " << line << "] Error: " << msg << endl;
    hadError = true;
}