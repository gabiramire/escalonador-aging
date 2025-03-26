#include "Escalonador.h"
//TODO: ainda sem tempo de chegada, mas preempção funciona
//TODO: implementar diferenças entre os tipos de perfis de processos

int main() {
    Escalonador escalonador;

    // processo: id, perfil, tempo_execucao, prioridade

    escalonador.adicionar_processo(Processo(1, CPU_BOUND, 6, 1)); // Baixa prioridade → começa executando
    escalonador.adicionar_processo(Processo(2, IO_BOUND, 5, 3));  // Média prioridade
    escalonador.adicionar_processo(Processo(3, MEMORY_BOUND, 4, 5)); // Alta prioridade → deve interromper o 1 logo
    escalonador.adicionar_processo(Processo(4, CPU_BOUND, 2, 2)); // Prioridade intermediária
    escalonador.adicionar_processo(Processo(5, IO_BOUND, 3, 4));  // Mais alta que 1 e 2
    escalonador.adicionar_processo(Processo(6, MEMORY_BOUND, 6, 3)); // Empata com o 2    

    escalonador.executar_simulacao();

    return 0;
}
