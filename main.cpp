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