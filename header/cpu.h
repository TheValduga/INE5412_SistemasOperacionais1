#ifndef CPU_H
#define CPU_H

#include <vector>
#include <string>
#include "hd.h"
#include "process.h"

using namespace std;

class CPU {
    public:
        int regs[6];
        int SP;
        int PC = 0;
        int ST;
        int tempo = 0;
        int trocaContexto = 0;
        HD disk;
        vector<Process> PSList;

        CPU(HD &hd);

        void restartCPU();
        void salvaContexto(int pidAntigo);
        void recuperaContexto(int pid);
        int scheduling(string tipo);
        void run(string tipo);

};

#endif //CPU_H