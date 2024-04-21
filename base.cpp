#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>
#include <vector>
#include <math.h>

using namespace std;



struct page {
    int regs[9];
};


class HD {
    public:
        vector<int> memory;
        int SP = 1;

        HD(int space) {
            memory.push_back(space);
        }

        int access(int location) {
            return memory[location];
        }

        page accessPage(int location) {
            page context;
            for (int i = 0; i < 9; i++) {
                context.regs[i] = memory[location+i];
            }
            return context;
        }

        int write(int location, int value) {
            if (location > memory[0]) {
                printf("Out of Bounds!");
            }
            if (location == 0 and SP > memory[0]) {
                printf("Memory out of Space!");
            }
            if (location == 0) {
                memory.push_back(value);
                SP += 1;
                return (SP-1);
            } else {
                memory[location] = value;
                return 0;
            }
        }

        void printHD() {
            int i;
            for (i = 0; i < SP; i++) {
                if (i % 10 == 0) {
                    cout << "\n";
                }
                cout << access(i) << '|';
            }
            for (i; i < memory[0]; i++) {
                cout << 0 << '|';
                if (i % 10 == 0) {
                    cout << "\n";
                }
            }
            cout << "\n";
        }
};

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

        Process(int pidN, int startN, int durationN, int priorityN, int periodN, int pstart, int pend, int repeats) {
            pid = pidN;
            start = startN;
            state = 'N';
            duration = durationN;
            durationMax = durationN;
            priority = priorityN;
            period = periodN;
            DLcur = periodN;
            DLmax = periodN;
            usedtime = 0;
            programStart = pstart;
            programEnd = pend;
            repeatable = repeats;
        }

        void restart() {
            duration = durationMax;
            repeatable -= 1;
            DLcur = DLmax;
        }

        void aging() {
            DLcur -= 1;
            if (DLcur < 0) {
                duration = 0;
            }
        }
};

class CPU {
    public:
        int regs[6];
        int SP;
        int PC = 0;
        int ST;
        int tempo = 0;
        HD disk;
        vector<Process> PSList;

        CPU(HD &hd) : disk(hd) {}

        void salvaContexto(int pidAntigo) {
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

        void recuperaContexto(int pid) {
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

        int scheduling(string tipo) {
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

        void run() {
            printf("\ntempo | P1 | P2 | P3 | P4 |\n");
            
            int pid;
            int pidAntigo;

            pid = scheduling("RM");
            PSList[pid].state = 'E';
            recuperaContexto(pid);

            while (true) {
                string resu("");


                for (int i = 3; i != 0; i--) {
                    int reg = disk.access(PC);
                    PC++;
                    int atrib = disk.access(PC);
                    PC++;
                    regs[reg] = atrib + PSList[pid].period - PSList[pid].DLcur;
                    if (PC > 16) {
                        PC = PSList[pid].start;
                    }
                }


                PSList[pid].duration -= 1;
                PSList[pid].usedtime += 1;
                if (PSList[pid].duration == 0) {
                    PSList[pid].state = 'C';
                    PSList[pid].end = tempo;
                }
                int pos = 0;
                for (Process p : PSList) {
                    p.aging();
                    PSList[pos] = p;
                    pos += 1;
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
                        case 'D':
                            resu.append(" XX |");
                            break;
                        case 'C':
                            resu.append(" VV |");
                            break;
                    }
                }
                printf("%02d-%02d |%s\n", tempo-1, tempo, resu.c_str());



                pidAntigo = pid;
                pid = scheduling("RM");
                if (pid == -2) {
                    break;
                }
                if ((PSList[pid].state == 'D' and PSList[pid].duration == 1) or (PSList[pid].state == 'P')) {
                    PSList[pid].state = 'E';
                }


                if (pid != pidAntigo) {
                    salvaContexto(pidAntigo);
                    recuperaContexto(pid);
                }
            }
        }
};




class File {
    public:
        File() {
            myfile.open("entrada.txt");
            if (!myfile.is_open()) {
                cout << "Erro ao abrir o arquivo!\n";
            }
        }

        void read_file(vector<Process> &p_) {
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
    private:
        ifstream myfile;
};




int main() {
    File f;
    vector<Process> Ps;
    f.read_file(Ps);

    HD hd1(100);

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
    INE5412.PSList = Ps;

    INE5412.run();

}
