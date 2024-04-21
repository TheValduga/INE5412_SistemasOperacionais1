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
    usedtime = 0;
    programStart = pstart;
    programEnd = pend;
    repeatable = repeats;
}

void Process::restart() {
    duration = durationMax;
    repeatable -= 1;
    DLcur = DLmax;
}

void Process::aging() {
    DLcur -= 1;
    if (DLcur < 0) {
        duration = 0;
    }
}