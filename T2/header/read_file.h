#ifndef READ_FILE_H
#define READ_FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


struct dados {
    int tipo;
    int tamanho;
    int bloco;
    int algoritmo;
};

class readFile {
    private:
        ifstream myfile;
        dados mydata;

    public:
        readFile();

        dados getMydata();
        void read_file();
};

#endif //READ_FILE_H