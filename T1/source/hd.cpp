#include <iostream>
#include "../header/hd.h"

using namespace std;

HD::HD(int space) { // A primeira posição da memória indica seu tamanho máximo
    memory.push_back(space);
}

int HD::access(int location) { // Retorna o int que estiver naquela posição na memória
    return memory[location];
}

page HD::accessPage(int location) { // Retorna uma página da memória, que é a posição fornecida e as 8 posições acima da memória
    page context;
    for (int i = 0; i < 9; i++) {
        context.regs[i] = memory[location+i];
    }
   return context;
}

int HD::write(int location, int value) {
    if (location > memory[0]) { // Erro para quando a posição da memória fornecida exceder o espaço
        printf("Out of Bounds!");
    }
    if (location == 0 and SP > memory[0]) { // Erro para quando a memória estiver cheia
        printf("Memory out of Space!");
    }
    if (location == 0) { // Se a localização for 0, significa que não importa onde será escrito esse dado, ele só precisa ser salvo
        memory.push_back(value);
        SP += 1;
        return (SP-1); // é retornado a posição em que esse dado foi guardado
    } else { // caso tenha um local fornecido, então atualiza essa posição com o valor atual
        memory[location] = value;
        return 0;
    }
}

void HD::printHD() { // Printa todos os valores da memória em linhas de 10 valores, com formatação de |
    int i;
    for (i = 0; i < SP; i++) {
        if (i % 10 == 0) {
            cout << "\n";
        }
        cout << access(i) << '|';
    }
    for (i; i < memory[0]; i++) { // O resto da memória em teoria terá lixo de memória, aqui nós representamos como 0 para indicar que não importa o que tem aqui
        cout << 0 << '|';
        if (i % 10 == 0) {
            cout << "\n";
        }
    }
    cout << "\n";
}