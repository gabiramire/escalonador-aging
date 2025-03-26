#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Processo.h"
#include <vector>

class Escalonador {
private:
    std::vector<Processo> fila_processos;
    std::vector<Processo> processos_concluidos;

public:
    void adicionar_processo(Processo p);
    Processo& selecionar_proximo_processo();
    void atualizar_prioridades();
    void simular_execucao(Processo& p);
    void atualizar_estado_processos();
    void executar_simulacao();
};

#endif // ESCALONADOR_H
