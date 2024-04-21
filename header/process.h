#ifndef PROCESS_H
#define PROCESS_H

class Process {
    public:
        int pid;
        int start;
        int end;
        int duration;
        int durationMax;
        int priority;
        char state; // (N)ovo, (P)ronto, (E)xecutando, (T)erminado
        int usedtime;
        int period;
        int DLcur; // Deadline Current (qual o limite em segundos atual para essa instância em específico ser entregue)
        int DLmax; // Deadline Max (qual o limite em segundos geral que esse processo tem para ser entregue)
        int repeatable;
        int context = 0; // Local da memória em que é salvo o contexto do processo
        int programStart; // Onde começa o código do programa na memória
        int programEnd; // Onde termina o código do programa na memória

        Process(int pidN, int startN, int durationN, int priorityN, int periodN, int pstart, int pend, int repeats);

        void restart();
        void aging();

};

#endif //PROCESS_H