# Escalonador Aging

## Descrição
Este projeto implementa um escalonador de processos utilizando a técnica de **Aging** para evitar a inanição de processos de baixa prioridade. O objetivo é garantir justiça no uso da CPU entre processos de diferentes prioridades. Processos de I/O possuem maior aumento de prioridade, visando utilização de entrada/saída no estado bloqueado enquanto outro processo utiliza o processador de forma ativa.

## Funcionalidades
- Gerenciamento de filas de processos por prioridade.
- Incremento de prioridade baseado no tempo de espera e tipo de processo.
- Simulação de execução de processos.

## Simulações
# I/O
Há um atributo de inicialização que estipula quantos bloqueios para I/O haverão na simulação. O mesmo depende da combinação com um evento aleatório disparado por randint (50% de chance), para dar certa aleatoriedade.
# Memória
Há um atributo de inicialização que estipula por quantos ciclos estaria havendo interação com a memória. Demais processamentos é denoatado por #.
# CPU
Cálculos pesados. Denotado por #.

** Seria legal agregar algo aos métodos de executar_ciclo para dar um enfeite **

## Output
- Símbolos por estado de processo:
    - Pronto: --
    - Executando: ## ou == (memory fetching simulation)
    - Bloqueado: O
