#include "Escalonador.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Escalonador::adicionar_processo(Processo p) {
    fila_processos.push_back(p);
}

// Aging: aumenta prioridade a cada 5 ciclos de espera
void Escalonador::atualizar_prioridades() {
    for (auto& processo : fila_processos) {
        processo.tempo_espera++;  

        if (processo.tempo_espera % 5 == 0) {
            processo.prioridade++;
            cout << "Aging aplicado ao processo " << processo.id 
                 << " (Nova prioridade: " << processo.prioridade << ")\n";
        }
    }
}

Processo& Escalonador::selecionar_proximo_processo() {
    if (fila_processos.empty()) {
        throw runtime_error("Nenhum processo na fila.");
    }

    sort(fila_processos.begin(), fila_processos.end(), 
        [](const Processo& a, const Processo& b) {
            return (a.prioridade > b.prioridade) || 
                   (a.prioridade == b.prioridade && a.id < b.id);
        });

    return fila_processos.front();
}

void Escalonador::simular_execucao(Processo& p) {
    cout << "Executando processo " << p.id 
         << " (Prioridade: " << p.prioridade 
         << ", Tempo restante: " << p.tempo_restante << ")\n";
    p.tempo_restante--;
}

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
        Processo &atual = selecionar_proximo_processo();
        
        cout << "\n>>> Selecionado processo " << atual.id 
             << " (Prioridade: " << atual.prioridade << ", Tempo restante: " << atual.tempo_restante << ")\n";

        for (int i = 0; i < 2 && atual.tempo_restante > 0; i++) {
            simular_execucao(atual);
            atual.tempo_espera = 0;

            bool preempte = false;
            for (auto &p : fila_processos) {
                if (p.id != atual.id && p.prioridade > atual.prioridade) {
                    preempte = true;
                    break;
                }
            }
            if (preempte) {
                cout << "Preempção! Processo " << atual.id << " pausado.\n";
                break;
            }
        }

        if (atual.tempo_restante <= 0) {
            cout << "Processo " << atual.id << " concluído.\n";
            processos_concluidos.push_back(atual);
            auto it = find_if(fila_processos.begin(), fila_processos.end(),
                [&atual](const Processo &p) { return p.id == atual.id; });
            if (it != fila_processos.end()) {
                fila_processos.erase(it);
            }
        }
    }
    cout << "\nTodos os processos foram concluídos!\n";
}
