#ifndef PROCESS_H
#define PROCESS_H

class Process {
    public:
        int pid;
        int start; // Tempo que o processo começa
        int end; // Tempo que o processo terminou
        int duration; // Quantos segundos de processamento falta para o processo terminar
        int durationMax; // Quantos segundos de processamento leva no total para ele terminar
        int priority;
        char state; // (N)ovo, (P)ronto, (E)xecutando, (T)erminado, (C) Executando e finalizando a execução, (D) Perdeu a Deadline
        int usedtime = 0; // Tempo em que estava sendo executado pelo processador
        int waittime = 0; // Tempo em que estava pronto para executar mas precisava esperar
        int DLlost = 0; // Quantas vezes perdeu a Deadline
        int period; // Tempo em que o processo repete
        int DLcur; // Deadline Current (quantos segundos faltam para esse processo em específico chegar na deadline)
        int DLmax; // Deadline Max (quantos segundos o processo tem do começo do período até a deadline)
        int repeatable; // Quantas vezes o processo ainda vai repetir (é um contador que reduz toda vez que uma nova instância do mesmo processo começar)
        int repeatableMax; // Quantas vezes o processo repete
        int context = 0; // Local da memória em que é salvo o contexto do processo (página). 0 = não possui uma página ainda.
        int programStart; // Posição inicial do "código" que o processo precisa executar
        int programEnd; // Posição final do "código".

        Process(int pidN, int startN, int durationN, int priorityN, int periodN, int pstart, int pend, int repeats);

        void restart(); // Reinicia o processo (deadline/duration) para indicar que é uma nova instância do processo periódico que inicio.
        void aging(); // Reduz o quanto falta para as deadlines em 1.

};

#endif //PROCESS_H