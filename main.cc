#include "escalonador.h"

int main() {
    srand(time(NULL));
    Escalonador escalonador;

    // CPU: id, tempo_chegada, tempo_execucao, prioridade
    // I/O: id, tempo_chega, tempo_execucao, prioridade, ciclos_bloqueado
    // Memoria: id, tempo_chegada, tempo_execucao, prioridade, ciclos_aguardar

    escalonador.adicionar_processo(new ProcessoCPU(1, 0, 4, 1));
    escalonador.adicionar_processo(new ProcessoIO(2, 1, 5, 1, 3));
    escalonador.adicionar_processo(new ProcessoMemoria(3, 2, 6, 3, 2));
    escalonador.adicionar_processo(new ProcessoMemoria(4, 5, 3, 2, 1));
    escalonador.adicionar_processo(new ProcessoIO(5, 7, 4, 1, 1));

    
    escalonador.executar_simulacao();


    return 0;
}
