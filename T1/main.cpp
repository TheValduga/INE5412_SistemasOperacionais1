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
    f.read_file(Ps); // Leitura dos processos do arquivo

    HD hd1(200); // Inicialização da memória com seu tamanho

    // Será colocado na memória instruções de "pseudo-código" que serão executados pela CPU por cada processo
    hd1.write(0, 0); hd1.write(0, 3); // As chamadas de função da esquerda guardam o registrador
    hd1.write(0, 1); hd1.write(0, 31); // As chamadas da direita guardam a atribuição
    hd1.write(0, 3); hd1.write(0, 62); // o primeiro valor é sempre 0 pois não importa em que lugar serão guardados esses valores
    hd1.write(0, 5); hd1.write(0, 70);
    hd1.write(0, 2); hd1.write(0, 51);
    hd1.write(0, 1); hd1.write(0, 37);
    hd1.write(0, 1); hd1.write(0, 845);
    hd1.write(0, 4); hd1.write(0, 23);
    hd1.SP = 17; // Atualiza manualmente o ponteiro da memória pra posição 17
    CPU INE5412(hd1); // Instancia a CPU com acesso a memória


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
    INE5412.restartCPU(); // Reinicia a CPU para que os valores antigos em relação aos processos não afete a nova execução
    hd1.SP = 17; // Atualiza também a memória para "apagar" as páginas antigas dos processos

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