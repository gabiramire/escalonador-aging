#include "escalonador.h"
#include <algorithm>
#include <iomanip>
#include <map>

void Escalonador::adicionar_processo(Processo* p) {
    p->ordem_chegada = proximo_num_ordem++;
    todos_processos.push_back(p);
}

void Escalonador::aplicar_aging(int tempo_atual, std::map<int, std::vector<std::string>>& eventos) {
    for (auto& p : fila_prontos) {
        if (p->estado == PRONTO) {
            p->tempo_espera++;
            if (p->tempo_espera % 3 == 0) {
                eventos[tempo_atual].push_back("P" + std::to_string(p->id) + "(" + std::to_string(p->prioridade) + "->" + std::to_string(p->prioridade + 1) + ");");
                p->prioridade++;
            }
        }
    }
}

void Escalonador::executar_simulacao() {
    std::cout << "\n=== Processos ===\n";
    for (size_t i = 0; i < todos_processos.size(); ++i) {
        std::cout << "P" << (i + 1) << ": " << todos_processos[i]->tipo() << std::endl;
        std::cout << "  prioridade inicial: " << todos_processos[i]->prioridade << std::endl;
    }

    int tempo_atual = 0;
    std::map<int, std::vector<std::string>> eventos_aging;
    std::map<int, std::map<int, std::string>> linha_tempo;

    Processo* executando = nullptr;

    while (processos_concluidos.size() < todos_processos.size()) {
        // Chegada de processos
        for (auto& p : todos_processos) {
            if (p->tempo_chegada == tempo_atual) {
                p->estado = PRONTO;
                fila_prontos.push_back(p);
            }
        }

        aplicar_aging(tempo_atual, eventos_aging);

        // Verifica se há processo executando e se terminou
        if (executando && executando->tempo_restante <= 0) {
            executando->estado = CONCLUIDO;
            processos_concluidos.push_back(executando);
            executando = nullptr;
        }

        // Verifica preempção
        if (executando && !fila_prontos.empty()) {
            auto it = std::max_element(fila_prontos.begin(), fila_prontos.end(), [&](Processo* a, Processo* b) {
                return (a->prioridade < b->prioridade) ||
                       (a->prioridade == b->prioridade && a->ordem_chegada > b->ordem_chegada);
            });
        
            Processo* candidato = *it;
            if (candidato->prioridade > executando->prioridade) {
                executando->estado = PRONTO;
                executando->tempo_espera = 0;
                fila_prontos.push_back(executando);
        
                executando = candidato;
                executando->estado = EXECUTANDO;
                fila_prontos.erase(std::remove(fila_prontos.begin(), fila_prontos.end(), executando), fila_prontos.end());
            }
        }
        

        // Se ninguém está executando, escolhe o processo de menor prioridade
        if (!executando && !fila_prontos.empty()) {
            auto it = std::max_element(fila_prontos.begin(), fila_prontos.end(), [](Processo* a, Processo* b) {
                return (a->prioridade < b->prioridade) ||
                       (a->prioridade == b->prioridade && a->ordem_chegada > b->ordem_chegada);  // desempate por ordem de chegada
            });
            executando = *it; 
            executando->estado = EXECUTANDO;
            fila_prontos.erase(std::remove(fila_prontos.begin(), fila_prontos.end(), executando), fila_prontos.end());
        }

        // Preenche a linha do tempo
        for (size_t i = 0; i < todos_processos.size(); ++i) {
            Processo* p = todos_processos[i];

            if (tempo_atual < p->tempo_chegada || p->estado == CONCLUIDO) {
                linha_tempo[tempo_atual][i] = "  ";
            } else if (p == executando) {
                bool executou = p->executar_ciclo();
                linha_tempo[tempo_atual][i] = executou ? "##" : "==";
            } else if (p->estado == PRONTO) {
                linha_tempo[tempo_atual][i] = "--";
            } else {
                linha_tempo[tempo_atual][i] = "  ";
            }
        }

        tempo_atual++;
    }

    imprimir_linha_tempo(linha_tempo, eventos_aging, tempo_atual);
    std::cout << "\nTodos os processos foram concluidos!\n";
}

void Escalonador::imprimir_linha_tempo(const std::map<int, std::map<int, std::string>>& linha_tempo, const std::map<int, std::vector<std::string>>& eventos, int tempo_total) {

    std::cout << "\n=== Linha do Tempo ===\n\n";
    std::cout << std::setw(6) << "Tempo";
    for (size_t i = 0; i < todos_processos.size(); ++i) {
        std::cout << " | P" << (i + 1);
    }
    std::cout << " | Eventos" << std::endl;

    for (int t = 0; t < tempo_total; ++t) {
        std::cout << std::setw(6) << t;
        for (size_t i = 0; i < todos_processos.size(); ++i) {
            std::string estado = linha_tempo.at(t).count(i) ? linha_tempo.at(t).at(i) : "  "; 
            std::cout << " | " << std::setw(2) << estado;
        }
        std::cout << " | ";
        if (eventos.count(t)) {
            for (const auto& e : eventos.at(t)) {
                std::cout << e << " ";
            }
        }
        std::cout << std::endl;
    }
}
