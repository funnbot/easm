#ifndef EASVM_VM_H
#define EASVM_VM_H

#include <string>
#include <fstream>

static bool hadError = false;

int execute(const std::vector<uint64_t> bytes);

bool openFile(const std::string& file, std::ifstream& ifile);
std::vector<uint64_t> readBytes(std::ifstream& ifile);
void error(const std::string& msg);

#endif