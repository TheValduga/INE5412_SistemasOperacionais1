#include <iostream>
#include "../header/hd.h"

using namespace std;

HD::HD(int space) {
    memory.push_back(space);
}

int HD::access(int location) {
    return memory[location];
}

page HD::accessPage(int location) {
    page context;
    for (int i = 0; i < 9; i++) {
        context.regs[i] = memory[location+i];
    }
   return context;
}

int HD::write(int location, int value) {
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

void HD::printHD() {
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