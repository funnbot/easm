#include <fstream>
#include <iostream>
#include <vector>
#include "vm.h"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 2) {
        ifstream ifile;
        if (openFile(argv[1], ifile)) {
            vector<uint64_t> bytes = readBytes(ifile);
            ifile.close();
            return execute(bytes);
        }
    } else {
        cout << "Usage: easvm [file].b" << endl;
    }

    return hadError;
}