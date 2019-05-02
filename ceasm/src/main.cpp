#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "ceasm.h"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 2) {
        ifstream ifile;
        if (CEASM::openFile(argv[1], ifile)) {
            vector<uint64_t> bytes = CEASM::compile(ifile);
            //CEASM::compileDebug(ifile);
			ifile.close();
			if (CEASM::hadError) return 1;

			if (CEASM::writeFile(bytes, argv[1])) {
				cout << "Wrote bytecode to \"" << argv[1] << ".b\"" << endl;
			}
        }
    } else {
        cout << "Usage: ceasm [file]" << endl;
    }

    return CEASM::hadError;
}