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

readFile::readFile() {
    myfile.open("entrada.txt");
    if (!myfile.is_open()) {
            cout << "Erro ao abrir o arquivo!\n";
    }
}

entrada readFile::getMydata() {
    return mydata;
}

void readFile::read_file() {
    if (!myfile.is_open()) {
        cout << "Arquivo não está aberto!" << endl;
    }

    string linha;
    int i = 0;
    while(getline(myfile, linha)) {
        linha[strcspn(linha.c_str(), "\n")] = 0;
        if (i < 4) {
            int aux = atoi(linha.c_str());
            mydata.dados.push_back(aux);
        } else {
            instrucoes atual;
            atual.tipo = linha[0];
            if (linha[0] == 'A') {
                string temp = "";
                for (int j = 2; j < linha.size(); j++) {
                    if (linha[j] != ' ') {
                        temp = temp + linha[j];
                    } else {
                        break;
                    }
                }
                atual.tamanho = atoi(temp.c_str());
            }
            atual.id = linha[linha.size() - 1];
            mydata.comando.push_back(atual);
        }
        i++;
    }
}


#endif //READ_FILE_H