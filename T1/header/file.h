#ifndef FILE_H
#define FILE_H

#include <fstream>
#include "process.h"
#include <vector>

using namespace std;

class File {
    private:
        ifstream myfile;

    public:
        File();

        void read_file(vector<Process> &p_);

};

#endif //FILE_H