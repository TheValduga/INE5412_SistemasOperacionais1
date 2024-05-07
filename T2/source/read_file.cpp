#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../header/read_file.h"

using namespace std;

readFile::readFile() {
    myfile.open("entrada.txt");
    if (!myfile.is_open()) {
            cout << "Erro ao abrir o arquivo!\n";
    }
}

dados readFile::getMydata() {
    return mydata;
}

void readFile::read_file() {
    int tipo, tamanho, bloco, algoritmo;

    if (!myfile.is_open()) {
        cout << "Arquivo não está aberto!" << endl;
    }
    while (myfile >> tipo >> tamanho >> bloco >> algoritmo) {
        mydata.tipo = tipo;
        mydata.tamanho = tamanho;
        mydata.bloco = bloco;
        mydata.algoritmo = algoritmo;
    }
}