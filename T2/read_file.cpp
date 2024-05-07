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
    vector<string> comandos;
};

class File {
    public:
        File() {
            myfile.open("entrada.txt");
            if (!myfile.is_open()) {
                cout << "Erro ao abrir o arquivo!\n";
            }
        }

        dados getMydata() {
            return mydata;
        }

        void read_file() {
            int tipo, tamanho, bloco, algoritmo;
            dados mydata;

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

    private:
        ifstream myfile;
        dados mydata;
};