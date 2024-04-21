#ifndef HD_H
#define HD_H

#include <vector>

struct page {
    int regs[9];
};

class HD {
    public:
        std::vector<int> memory;
        int SP = 1;

        HD(int space);  //construtor

        int access(int location);
        page accessPage(int location);
        int write(int location, int value);
        void printHD();
        
};

#endif //HD_H