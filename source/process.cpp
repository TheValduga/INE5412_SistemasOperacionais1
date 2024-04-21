#include "../header/process.h"
#include <iostream>

using namespace std;

Process::Process(int pidN, int startN, int durationN, int priorityN, int periodN, int pstart, int pend, int repeats) {
    pid = pidN;
    start = startN;
    state = 'N';
    duration = durationN;
    durationMax = durationN;
    priority = priorityN;
    period = periodN;
    DLcur = periodN;
    DLmax = periodN;
    programStart = pstart;
    programEnd = pend;
    repeatable = repeats;
    repeatableMax = repeats;
}

void Process::restart() { // Função para quando o periodo do processo passar
    duration = durationMax; // reinicia o tempo de execução que o processo precisa para Finalizar
    repeatable -= 1; // contador que mostra quantas vezes o processo precisa ser executado (quantos ciclos de periodo)
    DLcur = DLmax; // Atualiza a Deadline
}

void Process::aging() {
    DLcur -= 1; // A deadline é reduzida por um segundo
    if (DLcur <= 0) { // Se ela estiver em 0 ou abaixo significa que ela perdeu a Deadline
        duration = 0; // Evita que o processo seja executado depois de ter sido perdido
    }
}