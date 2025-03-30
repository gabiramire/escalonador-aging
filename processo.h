#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>
#include <vector>
using namespace std;

enum Estado { NOVO, PRONTO, EXECUTANDO, CONCLUIDO };

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
private:
    bool executando = true;
    int ciclos_parados = 0;

public:
    ProcessoIO(int id, int chegada, int tempo, int prioridade)
        : Processo(id, chegada, tempo, prioridade) {}

    bool executar_ciclo() override {
        if (estado != EXECUTANDO) return false;
    
        if (ciclos_parados == 0) {
            tempo_restante--;
            ciclos_parados = 1; 
            return true;
        }
        
        // bloqueia o processo por 1 ciclo
        ciclos_parados = 0;
        return false;
    }
        
    string tipo() const override {
        return "IO";
    }
};

class ProcessoMemoria : public Processo {
private:
    int ciclos_parados = 0;

public:
    ProcessoMemoria(int id, int chegada, int tempo, int prioridade)
        : Processo(id, chegada, tempo, prioridade) {}
        bool executar_ciclo() override {
            if (estado != EXECUTANDO) return false;
        
            if (ciclos_parados == 0) {
                tempo_restante--;
                ciclos_parados = 1;
                return true;
            }
        
            if (1 <= ciclos_parados && ciclos_parados <= 3) {
                ciclos_parados++;
                return false;
            }
        
            if (ciclos_parados == 4) {
                tempo_restante--;
                ciclos_parados = 1; 
                return true;
            }
        
            return false; 
        }        

    string tipo() const override {
        return "Mem";
    }
};

#endif
