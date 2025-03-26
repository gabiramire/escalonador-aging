#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Definição do perfil do processo
enum Perfil {
    CPU_BOUND,
    IO_BOUND,
    MEMORY_BOUND
};

// Estrutura de um processo
struct Processo {
    int id;                 // Identificador único
    Perfil perfil;          // Tipo do processo
    int tempo_execucao;     // Tempo total estimado
    int prioridade;         // Prioridade dinâmica
    int tempo_espera;       // Tempo aguardando na fila
    int tempo_restante;     // Tempo de execução restante

    // Construtor
    Processo(int _id, Perfil _perfil, int _tempo_execucao, int _prioridade)
        : id(_id), perfil(_perfil), tempo_execucao(_tempo_execucao), 
          prioridade(_prioridade), tempo_espera(0), tempo_restante(_tempo_execucao) {}
};

// Classe do escalonador
class Escalonador {
private:
    vector<Processo> fila_processos;
    vector<Processo> processos_concluidos;

public:
    void adicionar_processo(Processo p);
    Processo selecionar_proximo_processo();
    void atualizar_prioridades();
    void simular_execucao(Processo& p);
    void atualizar_estado_processos();
    void executar_simulacao();
};

// Adiciona um novo processo à fila
void Escalonador::adicionar_processo(Processo p) {
    fila_processos.push_back(p);
}

// Atualiza a prioridade dos processos (implementação de aging)
void Escalonador::atualizar_prioridades() {
    for (auto& processo : fila_processos) {
        processo.tempo_espera++;
        
        // Aging: aumenta prioridade a cada 5 ciclos de espera
        if (processo.tempo_espera % 5 == 0) {
            processo.prioridade++;
        }
    }
}

// Seleciona o próximo processo com maior prioridade
Processo Escalonador::selecionar_proximo_processo() {
    if (fila_processos.empty()) {
        throw runtime_error("Nenhum processo na fila.");
    }

    // Ordena a fila por prioridade e desempate por ordem de chegada (ID menor primeiro)
    sort(fila_processos.begin(), fila_processos.end(), 
        [](const Processo& a, const Processo& b) {
            return (a.prioridade > b.prioridade) || 
                   (a.prioridade == b.prioridade && a.id < b.id);
        });

    return fila_processos.front();
}

// Simula a execução do processo
void Escalonador::simular_execucao(Processo& p) {
    cout << "Executando processo " << p.id << " (Prioridade: " << p.prioridade << ")\n";
    
    // Simula execução reduzindo tempo restante
    p.tempo_restante--;

    // Verifica se o processo terminou
    if (p.tempo_restante <= 0) {
        processos_concluidos.push_back(p);
        cout << "Processo " << p.id << " concluído.\n";
    }
}

// Atualiza a fila removendo processos concluídos
void Escalonador::atualizar_estado_processos() {
    fila_processos.erase(
        remove_if(fila_processos.begin(), fila_processos.end(),
            [](const Processo& p) { return p.tempo_restante <= 0; }),
        fila_processos.end()
    );
}

void Escalonador::executar_simulacao() {
    while (!fila_processos.empty()) {
        atualizar_prioridades();

        // Selecionar índice do processo com maior prioridade
        int idx = 0;
        for (int i = 1; i < fila_processos.size(); ++i) {
            if (fila_processos[i].prioridade > fila_processos[idx].prioridade ||
                (fila_processos[i].prioridade == fila_processos[idx].prioridade && fila_processos[i].id < fila_processos[idx].id)) {
                idx = i;
            }
        }

        // Executa diretamente na fila
        Processo& atual = fila_processos[idx];

        cout << "Executando processo " << atual.id << " (Prioridade: " << atual.prioridade << ")\n";
        atual.tempo_restante--;

        if (atual.tempo_restante <= 0) {
            cout << "Processo " << atual.id << " concluído.\n";
            processos_concluidos.push_back(atual);
            fila_processos.erase(fila_processos.begin() + idx);
        }

        // Atualiza aging e espera dos outros
        atualizar_prioridades();
    }

    cout << "Todos os processos foram concluídos.\n";
}



// Teste principal
int main() {
    Escalonador escalonador;

    // Adicionando processos de teste
    escalonador.adicionar_processo(Processo(1, CPU_BOUND, 3, 2));
    escalonador.adicionar_processo(Processo(2, IO_BOUND, 5, 1));
    escalonador.adicionar_processo(Processo(3, MEMORY_BOUND, 4, 3));

    // Executando a simulação
    escalonador.executar_simulacao();

    return 0;
}

