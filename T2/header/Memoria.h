#ifndef MEMORIA_H
#define MEMORIA_H

#include <iostream>
#include <stdexcept>
#include "LinkedList.h"
#include "DoublyLinkedList.h"

using namespace std;

class memoria {
    public:
        memoria();
        void setup(int g, int t, int b, int a);
        int getAlgoritmo();
        void print_memoria();
        void alocar(int id, int tamanho);
        void desalocar(int id);
        void unir(int index); 
    
    private:
        int gerenciamento;
        int tamanho;
        int bloco_min;
        int algoritmo;
        structures::DoublyLinkedList<int> g_lista{};
};

memoria::memoria() {}

void memoria::setup(int g, int t, int b, int a) {
    gerenciamento = g;
    tamanho = t;
    bloco_min = b;
    algoritmo = a;

    g_lista.push_back(-1, 0, tamanho/bloco_min);
}

int memoria::getAlgoritmo() {
    return algoritmo;
}

void memoria::print_memoria() {
    g_lista.print();
}

void memoria::alocar(int id, int tamanho) {
    // por enquanto só first fit e lista duplamente encadeada
    int size = tamanho;
    if (tamanho % bloco_min != 0) {
        size = tamanho + (bloco_min - (tamanho % bloco_min));
    }
    size = size / bloco_min;
    size_t i;
    if (getAlgoritmo() == 2) {
        i = g_lista.firstfit(size);
    } else if (getAlgoritmo() == 1) {
        i = g_lista.bestfit(size);
    }
    if (i == -1) {
        cout << "Nenhum espaço encontrado para ID: " << id << endl;;
    } else {
        int livre = g_lista.at_size(i);
        int end = g_lista.at_end(i);
        g_lista.insert(id, end, size, i);

        if (livre - size > 0 ) {
            g_lista.insert(-1, end + size, livre - size, i+1);
            g_lista.pop(i+2);
        } else {
            g_lista.pop(i+1);
        }
    }
}

void memoria::desalocar(int id) {
    size_t local = g_lista.find(id);
    int tamanho = g_lista.at_size(local);
    int endereco = g_lista.at_end(local);
    g_lista.insert(-1,endereco,tamanho, local);
    g_lista.pop(local+1);
    unir(local);
}

void memoria::unir(int index) {
    int i = 0;
    int save = g_lista.at_size(index);
    int tamanho = g_lista.at_size(index);
    int endereco = g_lista.at_end(index);
    if (g_lista.at(index+1u) == -1) {
        tamanho = tamanho + g_lista.at_size(index + 1u);
        g_lista.pop(index);
        g_lista.pop(index);
        g_lista.insert(-1, endereco, tamanho, index);

    }
    if (index > 0u) {
        if (g_lista.at(index-1u) == -1) {
            tamanho = tamanho + g_lista.at_size(index - 1u);
            endereco = g_lista.at_end(index-1u);
            g_lista.insert(-1, endereco, tamanho, index-1u);
            g_lista.pop(index);
            g_lista.pop(index);
        }
    }
}

#endif