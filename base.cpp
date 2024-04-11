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
        int duration;
        int priority;
        int deadline;
};

struct scheduler_turn {
    bool flag;
    bool full;
};

struct planning {
    int pid;
    int tempo;
    int deadline;
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

void printing(int &tempo,vector<int> &result, int &posR) {
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
                    if (not full) {
                        t.pid = p.pid;
                        t.tempo = tempo;
                        t.duration = p.duration;
                        t.deadline = p.deadline;
                        posR = pos;
                        full = true;
                        result.push_back(posR);
                    } else if (p.deadline < t.deadline) {
                        t.pid = p.pid;
                        t.tempo = tempo;
                        t.duration = p.duration;
                        t.deadline = p.deadline;
                        posR = pos;
                        result.push_back(posR);
                    }
                }
            }
            pos += 1;
        }

        plan.push_back(t);
        list<Process>::iterator it = Ps.begin();
        advance(it, posR);
        (*it).duration -= 1;
        tempo += 1;

        printing(tempo, result, posR);

        sleep(1);
    }
}



int main() {
    list<Process> Ps;

    Process P1;
    P1.pid = 0;
    P1.start = 0;
    P1.duration = 5;
    P1.priority = 2;
    P1.deadline = 10 + P1.start;
    Ps.push_back(P1);

    Process P2;
    P2.pid = 1;
    P2.start = 0;
    P2.duration = 2;
    P2.priority = 3;
    P2.deadline = 20 + P2.start;
    Ps.push_back(P2);

    Process P3;
    P3.pid = 2;
    P3.start = 1;
    P3.duration = 4;
    P3.priority = 1;
    P3.deadline = 15 + P3.start;
    Ps.push_back(P3);

    Process P4;
    P4.pid = 3;
    P4.start = 3;
    P4.duration = 3;
    P4.priority = 4;
    P4.deadline = 18 + P4.start;
    Ps.push_back(P4);


    list<planning> plan;
    // roundRobin(Ps, plan);
    EDF(Ps, plan);




    /*list<planning>::iterator it;
    for (it = plan.begin(); it != plan.end(); ++it) {
        cout << it->tempo << '|' << it->pid << '\t';
    }*/
}










