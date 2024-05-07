#include <iostream>
#include <unistd.h>
#include "../header/cpu.h"

using namespace std;

CPU::CPU(HD &hd) : disk(hd) {}

void CPU::salvaContexto(int pidAntigo) { // Pid do processo que vai ser guardado na memória
    int pos; // posição da memória
    if (PSList[pidAntigo].context != 0) { // Caso o processo em específico já tenha uma página na memória com seu contexto, atualiza a página
        pos = PSList[pidAntigo].context;
        disk.write(pos, regs[0]);
        disk.write(pos+1, regs[1]);
        disk.write(pos+2, regs[2]);
        disk.write(pos+3, regs[3]);
        disk.write(pos+4, regs[4]);
        disk.write(pos+5, regs[5]);
        disk.write(pos+6, SP);
        disk.write(pos+7, PC);
        disk.write(pos+8, ST);
    } else { // Caso o processo não tenha uma página, escreve no disco na posição livre.
        pos = disk.write(0, regs[0]);
        disk.write(0, regs[1]);
        disk.write(0, regs[2]);
        disk.write(0, regs[3]);
        disk.write(0, regs[4]);
        disk.write(0, regs[5]);
        disk.write(0, SP);
        disk.write(0, PC);
        disk.write(0, ST);
        PSList[pidAntigo].context = pos; // Guarda no processo a posição da memória em que sua página está.
    }
}

void CPU::restartCPU() {
    regs[0] = 0;
    regs[1] = 0;
    regs[2] = 0;
    regs[3] = 0;
    regs[4] = 0;
    regs[5] = 0;
    SP = 0;
    PC = 0;
    ST = 0;
    tempo = 0;
    trocaContexto = 0;
}

void CPU::recuperaContexto(int pid) { // Pid do processo que vai ser lido da memória
    trocaContexto += 1; // Contador de vezes que a CPU trocou de contexto
    int pos = PSList[pid].context; // context é a posição da memória que está o começo da página
    page context;
    if (pos == 0) { // Se for 0, significa que esse processo não tem um contexto prévio, ou seja, é a primeira vez executando ele
        for (int i = 0; i < 6; i++) { // Reinicia os Registradores
            regs[i] = 0;
        }
        SP = 0;
        PC = 0 + PSList[pid].programStart; // Program Counter vai estar apontando pro começo das instruções do processo na memória
        ST = 0;
    } else { // Caso o processo tenha um contexto guardado, busca a página na memória e atualiza os registradores
        context = disk.accessPage(pos);
        regs[0] = context.regs[0];
        regs[1] = context.regs[1];
        regs[2] = context.regs[2];
        regs[3] = context.regs[3];
        regs[4] = context.regs[4];
        regs[5] = context.regs[5];
        SP = context.regs[6];
        PC = context.regs[7];
        ST = context.regs[8];
    }
}

int CPU::scheduling(string tipo) { // tipo indica se é "RM" ou "EDF"
    int pid = -2;
    // pid == -2 significa que não há mais processos a serem executados, e a CPU pode encerrar
    // pid == -1 significa que há processos a serem executados, mas eles ainda não começaram
    // pid >= 0 é o pid do processo mais prioritário escolhido pelo escalonador
    int pos = 0;
    int menor = 0; // indica qual o menor valor de (período no RM/Deadline atual no EDF) encontrado entre os processos

    for (Process p : PSList) { // para cada processo na lista
        if (pid == -2 and p.repeatable > 0) { // Se ainda falta alguma instância dele para ser executada, não termina a execução do processador
            pid = -1;
        }
        if (tempo % p.period == 0 and p.repeatable > 0 and tempo != 0) {
            // Caso algum processo tenha atingido seu periodo, e ainda falta ser repetido
            // Reinicia as variáveis de para indicar que é uma "nova instância" do processo
            p.restart();
        }
        if (p.duration > 0) {
            if (p.start <= tempo) {
                p.state = 'P'; // se o processo falta ser executado, e já inicio, ele automaticamente está no estado Pronto
                if (pid == -1 or pid == -2) { // se nenhum processo foi escolhido até agora, escolhe o atual
                    if (tipo == "EDF") {
                        menor = p.DLcur; // No caso do EDF, a variável menor irá representar a menor deadline encontrada
                    } else {
                        menor = p.period; // No RM, será o menor período encontrado
                    }
                    pid = pos; // pid será setado como o processo atual
                } else if ((p.DLcur < menor and tipo == "EDF") or (p.period < menor and tipo == "RM")) {
                    // No caso do EDF, se a Deadline do processo atual for menor que a deadline do processo escolhido, então escolhe o atual
                    // No caso do RM, se o período do processo atual for menor que o período do processo escolhido, então escolhe o atual 
                    if (tipo == "EDF") {
                        menor = p.DLcur;
                    } else {
                        menor = p.period;
                    }
                    pid = pos;
                }
            }
        } else {
            // se o estado não precisa ser executado, ele está no estado Terminado.
            p.state = 'T';
        }
        
        
        if (((tempo+1) % p.period == 0) and p.duration > 0) {
            // Se falta apenas 1 segundo para a deadline do processo acabar, e ainda resta tempo de execução
            // O processo vai para o estado de Deadline perdida
            p.state = 'D';
            // Caso falte apenas 1 segundo de execução para ele terminar, e esse segundo ocorrer agora
            // Ele vai para o estado de Finalizado futuramente.
        }
        PSList[pos] = p;
        pos += 1;
    }
    return pid;
}

void CPU::run(string tipo) {
    string a = "\ntempo |"; // Header do diagrama de execução
    for (int i = 0; i < PSList.size();i++) {
        a.append(" P" + to_string(i+1) + " |"); // Para cada processo irá colocar um P1, P2, P3... no header
    }
    a.append("\n");
    printf("%s", a.c_str());
    
    int pid; // processo atual
    int pidAntigo; // processo antigo (usado para salvar o contexto dele)

    pid = scheduling(tipo); // executa o escalonamento inicial
    PSList[pid].state = 'E'; // Seta o processo que vai ser executado como E
    recuperaContexto(pid); // Como nenhum processo foi inciado até agora, essa chamada vai apenas inicializar os registradores como 0

    while (true) {
        string resu(""); // string em que as informações vao ser concatenadas para o diagrama de execução

        if (pid != -1) { // caso tenha algum processo a ser executado (e não estejam todos finalizados ou aguardando serem iniciados)
            
            // Esse for é uma simulação da execução da CPU, aqui ela irá usar o PC para buscar instruções da memória
            for (int i = 3; i != 0; i--) {
                // Vai executar 3 instruções por segundo (valor arbitrário)
                // Todos os processos usam o mesmo código fonte
                // Quando o processo chega no final do seu código, ele recomeça do inicio
                if (PC > PSList[pid].programEnd) {
                    PC = PSList[pid].programStart;
                }
                int reg = disk.access(PC); // O primeiro valor que ele pega é qual dos 6 registradores vai receber a atribuição
                PC++;
                int atrib = disk.access(PC); // O segundo valor é o que é atribuido ao registrador
                PC++;
                regs[reg] = atrib + PSList[pid].period - PSList[pid].DLcur; // Alguns valores do processo são usados para gerar "aleatoriedade" nos valores
            }

            // Atualiza os valores do processo (pid) em relação a sua execução
            PSList[pid].duration -= 1;
            PSList[pid].usedtime += 1;
            if (PSList[pid].duration == 0) { // Se o processo terminou de executar
                PSList[pid].state = 'C'; // Coloca ele no estado de Finalizado
                if (PSList[pid].repeatable == 0) { // Se ele não vai mais repetir seu periodo
                    PSList[pid].end = tempo; // Marca nele o tempo em que ele finalizou
                }
            }
            int pos = 0;
            for (Process p : PSList) { // Para os outros processos que não são o executado
                if (p.state == 'P') { // Se ele está pronto, adiciona 1 segundo na variável que guarda quanto tempo ele ficou esperando
                    p.waittime += 1;
                } else if (p.state == 'D') { // Se ele está no estado de Deadline Perdida, guarda a informação de quantas deadlines ele perdeu
                    p.DLlost += 1;
                    p.end = tempo; // O tempo final de execução caso esse seja a última instância desse processo. Se não for, esse valor vai ser atualizado na próxima iteração
                }
                p.aging(); // reduz em 1 segundo as deadlines de todos os processos. caso algum chegue em 0, significa que ele não deve ser mais executado pois ele perdeu a entrega
                PSList[pos] = p;
                pos += 1;
            }
        }

        sleep(1);
        tempo += 1;


        for (Process p : PSList) {
            switch (p.state) {
                case 'T':
                case 'N':
                    resu.append("    |");
                    break;
                case 'P':
                    resu.append(" -- |");
                    break;
                case 'E':
                    resu.append(" ## |");
                    break;
                case 'D': // Estado em que perdeu a Deadline (pode ser equivalente ao estado P, ou ao estado E)
                    resu.append(" XX |");
                    break;
                case 'C': // Estado em que finalizou sua execução (equivalente ao estado E)
                    resu.append(" VV |");
                    break;
            }
        }
        printf("%02d-%02d |%s\n", tempo-1, tempo, resu.c_str());



        if (pid != -1) {
            pidAntigo = pid; // Se o escalonador retornou que não tem nenhum processo para ser executado agora, então não troca o pidAntigo, e não troca o contexto ainda
        }
        pid = scheduling(tipo); // faz o escalonamento
        if (pid == -2) { // Se o pid == -2, significa que não tem mais processos para serem executados, finaliza a CPU
            break;
        }
        if (pid != -1) { // Se tem algum processo pra ser executado agora
            if ((PSList[pid].state == 'D' and PSList[pid].duration == 1) or (PSList[pid].state == 'P')) {
                // pid = processo escolhido
                // Caso o processo escolhido vá perder sua deadline no próximo segundo, mas ele só precisa ser executado por mais 1 segundo para Finalizar
                // Então ele não esta no estado de (D)eadline perdida, e sim no estado de (E)xecução
                // Caso ele esteja no estado Pronto, ele também passa para o (E)xecução
                PSList[pid].state = 'E';
            }
            // Caso um processo D seja escolhido pelo escalonador, mas ele não tem como entregar até sua deadline por faltar mais que 1 segundo, ele ainda será executado, mas aparecerá como XX no log
            if (pid != pidAntigo) { // Se o pid escolhido é diferente do anterior, então houve uma troca de processo
                salvaContexto(pidAntigo); // Faz a troca de contexto entre os dois processos
                recuperaContexto(pid);
            }
        }
    }
}