#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

// BLOQUEADO adicionado para operações de I/O
enum Estado { NOVO, PRONTO, EXECUTANDO, CONCLUIDO, BLOQUEADO };

class Processo {
public:
    int id;
    int tempo_chegada;
    int tempo_execucao;
    int prioridade;
    int tempo_espera;
    int tempo_restante;
    int ordem_chegada;
    Estado estado;
    vector<string> linha_tempo;

    Processo(int _id, int _chegada, int _execucao, int _prioridade)
        : id(_id), tempo_chegada(_chegada), tempo_execucao(_execucao),
          prioridade(_prioridade), tempo_espera(0), tempo_restante(_execucao),
          ordem_chegada(-1), estado(NOVO) {}

    virtual ~Processo() = default;

    virtual bool executar_ciclo() = 0;
    virtual string tipo() const = 0;

    void marcar_estado(const string& estado_str) {
        linha_tempo.push_back(estado_str);
    }

    string nome_formatado() const {
        string label = "P" + to_string(id) + "-" + tipo();
        return label + string(8 - label.size(), ' ');
    }
};

class ProcessoCPU : public Processo {
public:
    ProcessoCPU(int id, int chegada, int tempo, int prioridade)
        : Processo(id, chegada, tempo, prioridade) {}

    bool executar_ciclo() override {
        tempo_restante--;
        return true;
    }

    string tipo() const override {
        return "CPU";
    }
};

class ProcessoIO : public Processo {
 public:
    int bloqueios = 0;
    bool executando = true;
    ProcessoIO(int id, int chegada, int tempo, int prioridade, int bloqueios)
    : Processo(id, chegada, tempo, prioridade),
    bloqueios(bloqueios) {}
    
    bool executar_ciclo() override {
        if (estado != EXECUTANDO) return false;
        
        tempo_restante--;
        
        // // Simula bloqueio de I/O
        // if (bloqueios > 0 && rand() % 2 == 0) {  // Se tiver bloqueios restantes e for o momento (aleatório)
        //     bloqueios--;
        //     estado = BLOQUEADO;
        //     cout << "teste \n";
        //     return true;
        // }
        return true;
    }
        
    string tipo() const override {
        return "IO";
    }
};

class ProcessoMemoria : public Processo {
 public:
    int ciclos_aguardar = 0;

    ProcessoMemoria(int id, int chegada, int tempo, int prioridade, int ciclos_aguardar)
        : Processo(id, chegada, tempo, prioridade),
          ciclos_aguardar(ciclos_aguardar) {}

    bool executar_ciclo() override {
        if (estado != EXECUTANDO) return false;
    
        /*
        Simula a espera das operações de memória.
        */

        if (ciclos_aguardar > 0) {
            ciclos_aguardar--;
            tempo_restante--;
            return false;
        }

        tempo_restante--;
    
        return true; 
    }        

    string tipo() const override {
        return "Mem";
    }
};

#endif
