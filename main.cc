#include "escalonador.h"

int main() {
    Escalonador escalonador;

    //Processo: id, tempo_chegada, tempo_execucao, prioridade

    escalonador.adicionar_processo(new ProcessoCPU(1, 0, 4, 2));
    escalonador.adicionar_processo(new ProcessoCPU(2, 1, 5, 1));
    escalonador.adicionar_processo(new ProcessoCPU(3, 2, 6, 3));
    escalonador.adicionar_processo(new ProcessoCPU(4, 5, 3, 2));
    escalonador.adicionar_processo(new ProcessoCPU(5, 7, 4, 1));
    
    // escalonador.adicionar_processo(new ProcessoIO(2, 1, 5, 1));
    // escalonador.adicionar_processo(new ProcessoMemoria(3, 2, 6, 3));
    // escalonador.adicionar_processo(new ProcessoMemoria(4, 5, 3, 2));
    // escalonador.adicionar_processo(new ProcessoIO(5, 7, 4, 1));

    escalonador.executar_simulacao();

    return 0;
}
