#include <iostream>

#include "header/read_file.h"
#include "source/read_file.cpp"


int main() {
    readFile f;
    f.read_file();
    dados mydata = f.getMydata();

    printf("%d, %d, %d, %d", mydata.tipo, mydata.tamanho, mydata.bloco, mydata.algoritmo);

    return 0;
}