#ifndef PROCESSO_H
#define PROCESSO_H

#include <iostream>

enum Perfil {
    CPU_BOUND,
    IO_BOUND,
    MEMORY_BOUND
};

struct Processo {
    int id;                 
    Perfil perfil;          
    int tempo_execucao;     
    int prioridade;         
    int tempo_espera;       
    int tempo_restante;     

    Processo(int _id, Perfil _perfil, int _tempo_execucao, int _prioridade)
        : id(_id), perfil(_perfil), tempo_execucao(_tempo_execucao), 
          prioridade(_prioridade), tempo_espera(0), tempo_restante(_tempo_execucao) {}
};

#endif // PROCESSO_H
