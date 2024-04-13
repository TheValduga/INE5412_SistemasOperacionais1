#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>
#include <vector>



using namespace std;

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
        int DLcur; // Deadline Current
        int DLmax; // Deadline Max
        int queue; // quando o processo já passou do periodo e existe outra instancia esperando execução
        Process(int pidN, int startN, int durationN, int priorityN, int periodN) {
            pid = pidN;
            start = startN;
            state = 'N';
            duration = durationN;
            durationMax = durationN;
            priority = priorityN;
            period = periodN;
            DLcur = periodN;
            DLmax = periodN;
            queue = 0;
            usedtime = 0;
        }
};

struct scheduler_turn {
    bool flag;
    bool full;
};

struct planning {
    int pid;
    int tempo;
    int DLcur;
    int duration;
};



void roundRobin(list<Process> &Ps, list<planning> &plan) {
    int tempo = 0;
    bool flag = true;
    bool timeflag;
    int pos;

    while (flag) {
        flag = false;
        timeflag = true;
        pos = 0;
        for (Process p : Ps) {
            if (p.duration > 0) {
                flag = true;
                if (p.start <= tempo) {
                    planning t;
                    t.pid = p.pid;
                    t.tempo = tempo;
                    plan.push_back(t);

                    list<Process>::iterator it = Ps.begin();
                    advance(it, pos);
                    (*it).duration -= 1;
                    tempo += 1;
                    timeflag = false;
                }
            }
            pos += 1;
        }
        if (timeflag) {
            tempo += 1;
        }
    }
}

void printing(int &tempo, vector<int> &result, int &posR) {
    string resu("");
    int aux = 0;
    int proc = 0;
    while (!(result.empty())){
        if (result[0] != aux) {
            resu.append("    |");
            aux++;
            proc++;
        } else {
            if (result[0] == posR) {
                resu.append(" ## |");
            } else {
                resu.append(" -- |");
            }
            aux++;
            result.erase(result.begin());
            proc++;
        }
    }
    if (proc < 4){
        for(int i = 0; i < (4-proc); i++){
            resu.append("    |");
        }
    }
    printf("%02d-%02d |%s\n", tempo-1,tempo, resu.c_str());
}

void EDF(list<Process> &Ps, list<planning> &plan) {
    int tempo = 0;
    bool flag = true;
    bool timeflag;
    int pos;
    int posR;

    printf("\ntempo | P1 | P2 | P3 | P4 |\n");
    while (flag) {
        flag = false;
        timeflag = true;
        pos = 0;
        planning t;
        vector<int> result;
        bool full = false;
        for (Process p : Ps) {
            if (p.duration > 0) {
                flag = true;
                if (p.start <= tempo) {
                    p.state = 'P';
                    result.push_back(pos);
                    if (not full) {
                        t.pid = p.pid;
                        t.tempo = tempo;
                        t.duration = p.duration;
                        t.DLcur = p.DLcur;
                        posR = pos;
                        full = true;
                    } else if (p.DLcur < t.DLcur) {
                        t.pid = p.pid;
                        t.tempo = tempo;
                        t.duration = p.duration;
                        t.DLcur = p.DLcur;
                        posR = pos;
                    }
                }
            } else if (p.queue > 0) {
                result.push_back(pos);
                p.duration = p.durationMax;
                p.DLcur += p.DLmax;
                p.queue -= 1;
            } else {
                p.state = 'T';
            }
            if (tempo % p.period == 0 && tempo != 0) {
                if (p.duration != 0) {
                    p.queue += 1;
                } else {
                    p.duration = p.durationMax;
                }
            }
            list<Process>::iterator it2 = Ps.begin();
            advance(it2, pos);
            (*it2) = p;
            pos += 1;
        }


        plan.push_back(t);
        list<Process>::iterator it = Ps.begin();
        advance(it, posR);
        (*it).duration -= 1;
        (*it).state = 'E';
        tempo += 1;

        printing(tempo, result, posR);

        sleep(0.1);
    }
}



int main() {
    list<Process> Ps;

    Process P1(0, 0, 5, 1, 20);
    Ps.push_back(P1);

    Process P2(1, 0, 2, 4, 8);
    Ps.push_back(P2);

    Process P3(2, 1, 4, 2, 16);
    Ps.push_back(P3);

    Process P4(3, 3, 3, 3, 12);
    Ps.push_back(P4);


    printf("%d", P1.duration);

    list<planning> plan;
    // roundRobin(Ps, plan);
    EDF(Ps, plan);




    /*list<planning>::iterator it;
    for (it = plan.begin(); it != plan.end(); ++it) {
        cout << it->tempo << '|' << it->pid << '\t';
    }*/
}










