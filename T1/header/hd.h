#ifndef HD_H
#define HD_H

#include <vector>

// estrutura que simula uma página de contexto, possui 9 valores inteiros
// Correspondentes aos 6 registradores principais e os 3 especias
struct page {
    int regs[9];
};


// Simulação da memória secundária
class HD {
    public:
        vector<int> memory; // vetor de inteiros que simula a memória
        int SP = 1; // valor que aponta para a próxima posição livre da memória

        HD(int space);  // Construtor: A posição 0 da memória guarda o valor do seu tamanho total

        int access(int location); // Acesso a uma posição da memória
        page accessPage(int location); // Acesso a uma página da memória (basicamente 9 acessos normais)
        int write(int location, int value); // Escreve numa posição da memória
        void printHD(); // Printa tudo que está dentro da memória formatado em 10 posições por linha
        
};

#endif //HD_H