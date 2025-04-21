#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "processo.h"
#include <vector>
#include <iostream>
#include <map>

class Escalonador {
private:
    std::vector<Processo*> todos_processos;
    std::vector<Processo*> fila_prontos;
    std::vector<Processo*> processos_concluidos;
    std::vector<Processo*> processos_bloqueados;
    std::vector<int> time_retornos;
    int proximo_num_ordem = 0;

public:
    void adicionar_processo(Processo* p);
    void executar_simulacao();
    void aplicar_aging(int tempo_atual, std::map<int, std::vector<std::string>>& eventos);
    void imprimir_linha_tempo(const std::map<int, std::map<int, std::string>>& linha_tempo,
                              const std::map<int, std::vector<std::string>>& eventos,
                              int tempo_total);
    void imprimir_retorno_medio() const;
};

#endif
