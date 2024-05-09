#include <iostream>

#include "header/read_file.h"
#include "source/read_file.cpp"


int main() {
    readFile f;
    f.read_file();
    entrada mydata = f.getMydata();

    //use os prints pra te ajudar a entender como esta a entrada
    /* a entrada esta colocada na forma de 2 vetores, um para os primeiros 4 dados para setar o algoritmo
       e outro que possui os comando ja separados em forma de uma struct (olhe o readfile.h, teremos que
       retirar essas structs em algum momento)*/

    // checagem dos valores de entrada, colocados em mydata.dados em ordem de entrada, que nem no arquivo
    for (int i = 0; i < 4; i++) printf("%d\n", mydata.dados.at(i));
    printf("\n");

    // checagem dos valores dos comandos
    for (int i = 0; i < mydata.comando.size(); i++) {
        printf("%c\n", mydata.comando.at(i).tipo);
        if (mydata.comando.at(i).tipo == 'A') {
            printf("%d\n", mydata.comando.at(i).tamanho);
        }
        printf("%c\n", mydata.comando.at(i).id);
        printf("\n");
    }

    return 0;
}