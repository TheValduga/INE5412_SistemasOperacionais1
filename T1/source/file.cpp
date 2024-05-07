#include <iostream>
#include "../header/file.h"

using namespace std;

File::File() {
    myfile.open("entrada.txt");
    if (!myfile.is_open()) {
        cout << "Erro ao abrir o arquivo!\n";
    }
}

void File::read_file(vector<Process> &p_) {
    int creation_date, duration, period, deadline, priority, repeticao;
    if (!myfile.is_open()) {
        cout << "Arquivo não está aberto!" << endl;
    }
    int pid = 0;
    while (myfile >> creation_date >> duration >> period >> deadline >> priority >> repeticao) {
        Process p = Process(pid, creation_date, duration, priority, period, 1, 15, repeticao);
        p_.push_back(p);
        pid++;
    }   
    cout << "Quantidade de processos lidos do arquivo: " << p_.size() << endl;
}
