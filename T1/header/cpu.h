#ifndef CPU_H
#define CPU_H

#include <vector>
#include <string>
#include "hd.h"
#include "process.h"

using namespace std;

// Representação do processador de um computador
class CPU {
    public:
        int regs[6]; // Registradores de uso geral
        int SP; // Stack Pointer
        int PC = 0; // Program Counter, indica qual a instrução que deve ser lida da memória
        int ST; // Status
        int tempo = 0; // Tempo total decorrido da simulação
        int trocaContexto = 0; // Quantas vezes houve uma troca de contexto (processo de salvar os registradores na memória e recuperar outros registradores da memória)
        HD disk; // Vetor de inteiros, que simula a memória secundária
        vector<Process> PSList; // Vetor de processos

        CPU(HD &hd); // Construtor

        void restartCPU(); // Reinicia as variáveis da classe para executar tanto o RM e o EDF.
        void salvaContexto(int pidAntigo); // Salva na memória os valores atuais dos registradores
        void recuperaContexto(int pid); // Trás da memória os valores dos registradores de um processo, e salva nos da CPU
        int scheduling(string tipo); // Função de escalonamento que 
        void run(string tipo); // função principal da CPU, é o que roda a maior parte

};

#endif //CPU_H