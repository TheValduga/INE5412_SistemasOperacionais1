#include <iostream>
#include <stdio.h>
#include "header/read_file.h"
#include "header/DoublyLinkedList.h"
#include "header/LinkedList.h"
#include "header/Memoria.h"


int main() {
    readFile f;
    f.read_file();
    entrada mydata = f.getMydata();
    int gerencia = mydata.dados.at(0);
    int tamanho = mydata.dados.at(1);
    int bloco_minimo = mydata.dados.at(2);
    int algoritmo = mydata.dados.at(3);
    
    memoria memoria;
    memoria.setup(gerencia, tamanho, bloco_minimo, algoritmo);

    for (int i = 0; i < mydata.comando.size(); i++) {
        char c = mydata.comando.at(i).tipo;
        char id = mydata.comando.at(i).id - '0';
        if (c == 'A') {
            memoria.alocar(id, mydata.comando.at(i).tamanho);
        } else if (c == 'D') {
            memoria.desalocar(id);
        }
    }
    memoria.print_memoria();

    return 0;
}