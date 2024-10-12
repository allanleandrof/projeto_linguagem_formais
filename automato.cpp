#include "automato.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <queue>

// Construtor
automato::automato() {}

// Getters
vector<string> automato::getAlfabeto() const {
    return alfabeto;
}

vector<string> automato::getEstados() const {
    return estados;
}

string automato::getEstadoInicial() const {
    return estado_inicial;
}

vector<string> automato::getEstadosFinais() const {
    return estados_finais;
}

vector<tuple<string, string, string>> automato::getTransicoes() const {
    return transicoes;
}

// Setters
void automato::setAlfabeto(const vector<string>& novoAlfabeto) {
    alfabeto = novoAlfabeto;
}

void automato::setEstados(const vector<string>& novosEstados) {
    estados = novosEstados;
}

void automato::setEstadoInicial(const string& novoEstadoInicial) {
    estado_inicial = novoEstadoInicial;
}

void automato::setEstadosFinais(const vector<string>& novosEstadosFinais) {
    estados_finais = novosEstadosFinais;
}

void automato::setTransicoes(const vector<tuple<string, string, string>>& novasTransicoes) {
    transicoes = novasTransicoes;
}

// Função para carregar o autômato de um arquivo
void automato::carregarAutomato(const string& arquivo) {
    ifstream file(arquivo);
    string linha;

    while (getline(file, linha)) {
        if (linha.find("alfabeto:") == 0) {
            linha = linha.substr(9); // Remove "alfabeto:"
            stringstream ss(linha);
            string simbolo;
            vector<string> novoAlfabeto;
            while (getline(ss, simbolo, ',')) {
                novoAlfabeto.push_back(simbolo);
            }
            setAlfabeto(novoAlfabeto);
        } else if (linha.find("estados:") == 0) {
            linha = linha.substr(8); // Remove "estados:"
            stringstream ss(linha);
            string estado;
            vector<string> novosEstados;
            while (getline(ss, estado, ',')) {
                novosEstados.push_back(estado);
            }
            setEstados(novosEstados);
        } else if (linha.find("inicial:") == 0) {
            setEstadoInicial(linha.substr(8)); // Remove "inicial:"
        } else if (linha.find("finais:") == 0) {
            linha = linha.substr(7); // Remove "finais:"
            stringstream ss(linha);
            string estado_final;
            vector<string> novosEstadosFinais;
            while (getline(ss, estado_final, ',')) {
                novosEstadosFinais.push_back(estado_final);
            }
            setEstadosFinais(novosEstadosFinais);
        } else if (linha.find("transicoes") == 0) {
            vector<tuple<string, string, string>> novasTransicoes;
            while (getline(file, linha)) {
                stringstream ss(linha);
                string q0, q1, simbolo;
                getline(ss, q0, ',');
                getline(ss, q1, ',');
                getline(ss, simbolo, ',');
                novasTransicoes.emplace_back(q0, q1, simbolo);
            }
            setTransicoes(novasTransicoes);
        }
    }
}

// Função para exibir o autômato
void automato::exibir() const {
    cout << "Alfabeto: ";
    for (const auto& simbolo : getAlfabeto()) {
        cout << simbolo << " ";
    }
    cout << "\nEstados: ";
    for (const auto& estado : getEstados()) {
        cout << estado << " ";
    }
    cout << "\nEstado Inicial: " << getEstadoInicial() << endl;
    cout << "Estados Finais: ";
    for (const auto& estado_final : getEstadosFinais()) {
        cout << estado_final << " ";
    }
    cout << "\nTransicoes: \n";
    for (const auto& transicao : getTransicoes()) {
        cout << get<0>(transicao) << " -> " << get<1>(transicao) << " [ " << get<2>(transicao) << " ]" << endl;
    }
}

// Função para verificar se é um AFD
bool automato::ehAFD() const {
    map<pair<string, string>, string> transicoes_vistas;

    for (int i = 0; i < getTransicoes().size(); i++) {
        string estado_atual = get<0>(getTransicoes()[i]);
        string estado_destino = get<1>(getTransicoes()[i]);
        string simbolo = get<2>(getTransicoes()[i]);

        if (simbolo == "") {
            return false;
        }

        pair<string, string> chave = make_pair(estado_atual, simbolo);
        if (transicoes_vistas.find(chave) != transicoes_vistas.end()) {
            return false;
        }

        transicoes_vistas[chave] = estado_destino;
    }

    return true;
}

// Função auxiliar para mover os estados
set<string> mover(const set<string>& conjunto_estados, const string& simbolo, const vector<tuple<string, string, string>>& transicoes) {
    set<string> novos_estados;
    for (int i = 0; i < conjunto_estados.size(); i++) {
        string estado = *next(conjunto_estados.begin(), i);
        for (int j = 0; j < transicoes.size(); j++) {
            if (get<0>(transicoes[j]) == estado && get<2>(transicoes[j]) == simbolo) {
                novos_estados.insert(get<1>(transicoes[j]));
            }
        }
    }
    return novos_estados;
}

// Função para transformar AFN em AFD
automato automato::transformarAFNparaAFD() const {
    automato afd;

    map<set<string>, string> novos_estados_map;
    vector<tuple<string, string, string>> novas_transicoes;
    vector<string> novos_estados_finais;
    queue<set<string>> fila_estados;

    set<string> estado_inicial_afd = { getEstadoInicial() };
    fila_estados.push(estado_inicial_afd);
    novos_estados_map[estado_inicial_afd] = getEstadoInicial();
    afd.setEstadoInicial(getEstadoInicial());

    while (!fila_estados.empty()) {
        set<string> estados_atual = fila_estados.front();
        fila_estados.pop();

        for (const auto& estado_final : getEstadosFinais()) {
            if (estados_atual.find(estado_final) != estados_atual.end()) {
                novos_estados_finais.push_back(novos_estados_map[estados_atual]);
                break;
            }
        }

        for (const auto& simbolo : getAlfabeto()) {
            set<string> novos_estados = mover(estados_atual, simbolo, getTransicoes());

            if (!novos_estados.empty()) {
                if (novos_estados_map.find(novos_estados) == novos_estados_map.end()) {
                    string novo_estado_nome = "q" + to_string(novos_estados_map.size());
                    novos_estados_map[novos_estados] = novo_estado_nome;
                    fila_estados.push(novos_estados);
                }

                novas_transicoes.emplace_back(novos_estados_map[estados_atual], novos_estados_map[novos_estados], simbolo);
            }
        }
    }

    vector<string> novosEstados;
    for (const auto& par : novos_estados_map) {
        novosEstados.push_back(par.second);
    }
    afd.setEstados(novosEstados);
    afd.setTransicoes(novas_transicoes);
    afd.setEstadosFinais(novos_estados_finais);
    afd.setAlfabeto(getAlfabeto());

    return afd;
}
