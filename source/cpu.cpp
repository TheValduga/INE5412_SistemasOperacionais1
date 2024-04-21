#include <iostream>
#include <unistd.h>
#include "../header/cpu.h"

using namespace std;

CPU::CPU(HD &hd) : disk(hd) {}

void CPU::salvaContexto(int pidAntigo) {
    int pos;
    if (PSList[pidAntigo].context != 0) {
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
    } else {
        pos = disk.write(0, regs[0]);
        disk.write(0, regs[1]);
        disk.write(0, regs[2]);
        disk.write(0, regs[3]);
        disk.write(0, regs[4]);
        disk.write(0, regs[5]);
        disk.write(0, SP);
        disk.write(0, PC);
        disk.write(0, ST);
        PSList[pidAntigo].context = pos;
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

void CPU::recuperaContexto(int pid) {
    trocaContexto += 1;
    int pos = PSList[pid].context;
    page context;
    if (pos == 0) {
        for (int i = 0; i < 6; i++) {
            regs[i] = 0;
        }
        SP = 0;
        PC = 0 + PSList[pid].programStart;
        ST = 0;
    } else {
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

int CPU::scheduling(string tipo) {
    int pid = -2;
    int pos = 0;
    int menor = 0;

    for (Process p : PSList) {
        if (pid == -2 and p.repeatable > 0) {
            pid = -1;
        }
        if (tempo % p.period == 0 and p.repeatable > 0 and tempo != 0) {
            p.restart();
        }
        if (p.duration > 0) {
            if (p.start <= tempo) {
                p.state = 'P';
                if (pid == -1 or pid == -2) {
                    if (tipo == "EDF") {
                        menor = p.DLcur;
                    } else {
                        menor = p.period;
                    }
                    pid = pos;
                } else if ((p.DLcur < menor and tipo == "EDF") or (p.period < menor and tipo == "RM")) {
                    if (tipo == "EDF") {
                        menor = p.DLcur;
                    } else {
                        menor = p.period;
                    }
                    pid = pos;
                }
            }
        } else {
            p.state = 'T';
        }
        
        
        if (((tempo+1) % p.period == 0) and p.duration > 0) {
            p.state = 'D';
        }
        PSList[pos] = p;
        pos += 1;
    }
    return pid;
}

void CPU::run(string tipo) {
    string a = "\ntempo |";
    for (int i = 0; i < PSList.size();i++) {
        a.append(" P" + to_string(i+1) + " |");
    }
    a.append("\n");
    printf("%s", a.c_str());
    
    int pid;
    int pidAntigo;

    pid = scheduling(tipo);
    PSList[pid].state = 'E';
    recuperaContexto(pid);

    while (true) {
        string resu("");

        if (pid != -1) {
            for (int i = 3; i != 0; i--) {
                if (PC > PSList[pid].programEnd) {
                    PC = PSList[pid].programStart;
                }
                int reg = disk.access(PC);
                PC++;
                int atrib = disk.access(PC);
                PC++;
                regs[reg] = atrib + PSList[pid].period - PSList[pid].DLcur;
            }


            PSList[pid].duration -= 1;
            PSList[pid].usedtime += 1;
            if (PSList[pid].duration == 0) {
                PSList[pid].state = 'C';
                if (PSList[pid].repeatable == 0) {
                    PSList[pid].end = tempo;
                }
            }
            int pos = 0;
            for (Process p : PSList) {
                if (p.state == 'P') {
                    p.waittime += 1;
                } else if (p.state == 'D') {
                    p.DLlost += 1;
                    p.end = tempo;
                }
                p.aging();
                PSList[pos] = p;
                pos += 1;
            }
        }

        sleep(0.1);
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
                case 'D':
                    resu.append(" XX |");
                    break;
                case 'C':
                    resu.append(" VV |");
                    break;
            }
        }
        printf("%02d-%02d |%s\n", tempo-1, tempo, resu.c_str());



        if (pid != -1) {
            pidAntigo = pid;
        }
        pid = scheduling(tipo);
        // if (tempo == 20 or tempo == 21) {
        //     printf("%d\n", pid);
        // }
        if (pid == -2) {
            break;
        }
        if (pid != -1) {
            if ((PSList[pid].state == 'D' and PSList[pid].duration == 1) or (PSList[pid].state == 'P')) {
                PSList[pid].state = 'E';
            }
            if (pid != pidAntigo) {
                salvaContexto(pidAntigo);
                recuperaContexto(pid);
            }
        }
    }
}
