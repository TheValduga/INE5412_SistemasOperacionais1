#include <iostream>
#include <vector>

#include "header/file.h"
#include "header/process.h"
#include "header/hd.h"
#include "header/cpu.h"
#include "source/file.cpp"
#include "source/process.cpp"
#include "source/hd.cpp"
#include "source/cpu.cpp"
#include <math.h>



int main() {
    File f;
    vector<Process> Ps;
    f.read_file(Ps);

    HD hd1(200);

    hd1.write(0, 0); hd1.write(0, 3);
    hd1.write(0, 1); hd1.write(0, 31);
    hd1.write(0, 3); hd1.write(0, 62);
    hd1.write(0, 5); hd1.write(0, 70);
    hd1.write(0, 2); hd1.write(0, 51);
    hd1.write(0, 1); hd1.write(0, 37);
    hd1.write(0, 1); hd1.write(0, 845);
    hd1.write(0, 4); hd1.write(0, 23);
    hd1.SP = 17;
    CPU INE5412(hd1);


    printf("---------- Algoritmo Rate Monotonic\n");
    INE5412.PSList = Ps;

    INE5412.run("RM");

    int turnAround = 0;
    for (Process p : INE5412.PSList) {
        printf("Processo %d: ", p.pid);
        printf("Levou %d segundos para terminar.\n", (p.end - p.start));
        printf("Esteve Pronto, em espera, por %d segundos.\n", (p.waittime));
        printf("Perdeu %d Deadlines.\n", (p.DLlost));
        printf("----------\n");
        turnAround += (p.end - p.start);
    }
    printf("Em média os processos levaram %.0f segundos para terminar.\n", round(turnAround/INE5412.PSList.size()));
    printf("Houveram %d trocas de contexto.\n", INE5412.trocaContexto);




    printf("\n\n---------- Algoritmo Earliest Deadline First\n");
    INE5412.restartCPU();
    hd1.SP = 17;

    INE5412.PSList = Ps;

    INE5412.run("EDF");

    turnAround = 0;
    for (Process p : INE5412.PSList) {
        printf("Processo %d: ", p.pid);
        printf("Levou %d segundos para terminar.\n", (p.end - p.start));
        printf("Esteve Pronto, em espera, por %d segundos.\n", (p.waittime));
        printf("Perdeu %d Deadlines.\n", (p.DLlost));
        printf("----------\n");
        turnAround += (p.end - p.start);
    }
    printf("Em média os processos levaram %.0f segundos para terminar.\n", round(turnAround/INE5412.PSList.size()));
    printf("Houveram %d trocas de contexto.\n", INE5412.trocaContexto);
}