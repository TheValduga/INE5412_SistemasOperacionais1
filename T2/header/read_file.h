#ifndef READ_FILE_H
#define READ_FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>

using namespace std;

    struct instrucoes {
        char tipo;
        int tamanho;
        char id;
    };

    struct entrada {
        vector<int> dados;
        vector<instrucoes> comando;
    };

class readFile {
    private:
        ifstream myfile;
        entrada mydata;

    public:
        readFile();

        entrada getMydata();
        void read_file();
};


#endif //READ_FILE_H