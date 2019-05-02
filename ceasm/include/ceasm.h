#ifndef EASM_H
#define EASM_H

#include <istream>
#include <string>
#include <vector>

class CEASM {
   public:
    static bool hadError;

    static std::vector<uint64_t> compile(std::istream& src);

    static void compileDebug(std::istream& src);

    static bool openFile(const std::string &file, std::ifstream& ifile);
    static bool writeFile(std::vector<uint64_t>& bytes, const std::string& file);

    static void error(const std::string& msg);
    static void error(const std::string& msg, int line);
};

#endif