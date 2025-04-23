#include "escalonador.h"

int main() {
    srand(time(NULL));
    Escalonador escalonador;

    // tempo_chegada, tempo_execucao, prioridade

    escalonador.adicionar_processo(new ProcessoCPU(4, 4, 1));
    escalonador.adicionar_processo(new ProcessoCPU(5, 5, 2));
    escalonador.adicionar_processo(new ProcessoIO(2, 5, 3));
    escalonador.adicionar_processo(new ProcessoIO(3, 4, 1));
    escalonador.adicionar_processo(new ProcessoMemoria(0, 6, 5));
    escalonador.adicionar_processo(new ProcessoMemoria(1, 3, 4));

    
    escalonador.executar_simulacao();


    return 0;
}
