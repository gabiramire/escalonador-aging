#include "escalonador.h"
#include <algorithm>
#include <iomanip>
#include <map>


void Escalonador::adicionar_processo(Processo* p) {
    p->ordem_chegada = proximo_num_ordem++;
    todos_processos.push_back(p);
}

/*
Agora os processos de I/O tem prioridade aumentada de forma mais rápida.
(Conforme teoria onde processos de I/O devem ter prioridade maior para 
irem logo para entrada/saída e ficarem no estado bloqueado até sairem)
*/
void Escalonador::aplicar_aging(int tempo_atual, std::map<int, std::vector<std::string>>& eventos) {
    for (auto& p : fila_prontos) {
        if (p->estado == PRONTO) {
            p->tempo_espera++;
            if (p->tempo_espera % 3 == 0) {
                int aumento;
                if (p->tipo() == "IO") {
                    aumento = 2; // Aumenta mais a prioridade dos processos de I/O
                } else {
                    aumento = 1;
                }
                eventos[tempo_atual].push_back("P" + std::to_string(p->id) + "(" + std::to_string(p->prioridade) + "->" + std::to_string(p->prioridade + aumento) + ");");
                p->prioridade = p->prioridade + aumento;
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

    std::vector<int> time_retornos;
    std::map<int, int> tempo_resposta;  // <id processo, tempo de resposta>

    Processo* executando = nullptr;

    while (processos_concluidos.size() < todos_processos.size()) {
        // Chegada de processos e atualização da lista dos bloqueados
        for (auto& p : todos_processos) {
            if (p->tempo_chegada == tempo_atual) {
                p->estado = PRONTO;
                fila_prontos.push_back(p);
            }
        }

        
        // Verifica se há processo executando; se terminou ou se deve bloquear
        if (executando) {
            if (executando->tempo_restante <= 0) {  // Fim de processo
                time_retornos.push_back(tempo_atual - executando->tempo_chegada);
                executando->estado = CONCLUIDO;
                processos_concluidos.push_back(executando);
                executando = nullptr;
            } else {

                // Se executando for um ProcessoIO, processo_io será != nullptr
                auto* processo_io = dynamic_cast<ProcessoIO*>(executando);
                if (processo_io && processo_io->bloqueios > 0 && rand() % 2 == 0) {
                    executando->estado = BLOQUEADO;
                    processo_io->bloqueios--;
                    processos_bloqueados.push_back(executando);
                    executando = nullptr;
                }
            }
        }

        // Verifica preempção
        if (executando && !fila_prontos.empty()) {

            /*
            Escolhe da fila de prontos o processo com *maior* prioridade.
            Em caso de empate de prioridade, a ordem de chegada que conta.
            */
           auto it = std::max_element(fila_prontos.begin(), fila_prontos.end(), [&](Processo* a, Processo* b) {
               return (a->prioridade < b->prioridade) ||
               (a->prioridade == b->prioridade && a->ordem_chegada > b->ordem_chegada);
            });
            
            // Preempção e troca de processo executando
            Processo* candidato = *it;

            // Se o candidato tiver igual prioridade, mas chegou antes, deve trocar também*
            if (candidato->prioridade > executando->prioridade ||
                (candidato->prioridade == executando->prioridade && 
                    candidato->ordem_chegada < executando->ordem_chegada)) {
        
                executando->estado = PRONTO;
                executando->tempo_espera = 0;
                fila_prontos.push_back(executando);
                
                executando = candidato;
                executando->estado = EXECUTANDO;
                fila_prontos.erase(std::remove(fila_prontos.begin(), fila_prontos.end(), executando), fila_prontos.end());
            }
        }
        
        // Se ninguém está executando, escolhe o processo de maior prioridade
        if (!executando && !fila_prontos.empty()) {
            auto it = std::max_element(fila_prontos.begin(), fila_prontos.end(), [](Processo* a, Processo* b) {
                return (a->prioridade < b->prioridade) ||
                (a->prioridade == b->prioridade && a->ordem_chegada > b->ordem_chegada);  // desempate por ordem de chegada
            });
            executando = *it; 
            executando->estado = EXECUTANDO;
            fila_prontos.erase(std::remove(fila_prontos.begin(), fila_prontos.end(), executando), fila_prontos.end());
        }
        
        aplicar_aging(tempo_atual, eventos_aging);
        
        // Preenche a linha do tempo e executa o ciclo do processo
        for (size_t i = 0; i < todos_processos.size(); ++i) {
            Processo* p = todos_processos[i];

            if (tempo_atual < p->tempo_chegada || p->estado == CONCLUIDO) {
                linha_tempo[tempo_atual][i] = "  ";
            } else if (p == executando) {
                bool executou = p->executar_ciclo();
                linha_tempo[tempo_atual][i] = executou ? "##" : "#";  // Revisar essa linha

                // Se o processo estiver começando execução agora
                if (executando->tempo_inicio_execucao == -1) {
                    executando->tempo_inicio_execucao = tempo_atual;
                    tempo_resposta.insert({executando->id, tempo_atual - executando->tempo_chegada});  // Tempo de resposta
                }

            } else if (p->estado == PRONTO) {
                linha_tempo[tempo_atual][i] = "--";
            } else if (p->estado == BLOQUEADO) {
                linha_tempo[tempo_atual][i] = "==";
            } else {
                linha_tempo[tempo_atual][i] = "  ";
            }
        }

        // Simula desbloqueio de processos de I/O
        for (auto& p : processos_bloqueados) {
            if (p->estado == BLOQUEADO && rand() % 2 == 0) { // Desbloqueio aleatório
                
                p->estado = PRONTO;
                fila_prontos.push_back(p);

                processos_bloqueados.erase(std::remove(processos_bloqueados.begin(), 
                                                        processos_bloqueados.end(), p), 
                                                        processos_bloqueados.end());
            }
        }

        tempo_atual++;
    }

    imprimir_linha_tempo(linha_tempo, eventos_aging, tempo_atual);
    std::cout << "\nTodos os processos foram concluidos!\n";
    imprimir_retorno_medio(time_retornos);
    imprimir_tempo_resposta(tempo_resposta);
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

void Escalonador::imprimir_retorno_medio(std::vector<int> time_retornos) const {
    if (time_retornos.empty()) {
        std::cout << "Nenhum processo foi concluído." << std::endl;
        return;
    }

    double soma = 0;
    for (int retorno : time_retornos) {
        soma += retorno;
    }
    double media = soma / time_retornos.size();
    std::cout << "\n === Tempo médio de retorno === \n" << media  << " ms"<< std::endl;
}

void Escalonador::imprimir_tempo_resposta(std::map<int, int> tempo_resposta) {
    std::cout << "\n=== Tempo de resposta ===\n";
    for (const auto& p : tempo_resposta) {
        std::cout << "P" << p.first << ": " << p.second << " ms" << std::endl;
    }
}