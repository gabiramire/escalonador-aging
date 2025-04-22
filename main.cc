#include "escalonador.h"

int main() {
    srand(time(NULL));
    Escalonador escalonador;

    // id, tempo_chegada, tempo_execucao, prioridade

    escalonador.adicionar_processo(new ProcessoCPU(1, 0, 4, 1));
    escalonador.adicionar_processo(new ProcessoCPU(2, 1, 5, 2));
    escalonador.adicionar_processo(new ProcessoIO(2, 2, 5, 3));
    escalonador.adicionar_processo(new ProcessoIO(5, 3, 4, 1));
    escalonador.adicionar_processo(new ProcessoMemoria(3, 3, 6, 3));
    escalonador.adicionar_processo(new ProcessoMemoria(4, 4, 3, 2));

    
    escalonador.executar_simulacao();


    return 0;
}
