#include "automato.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <queue>

automato::automato() {}

void automato::carregarAutomato(const string& arquivo) {
    ifstream file(arquivo);
    string linha;

    while (getline(file, linha)) {
        if (linha.find("alfabeto:") == 0) {
            linha = linha.substr(9); // Remove "alfabeto:"
            stringstream ss(linha);
            string simbolo;
            while (getline(ss, simbolo, ',')) {
                alfabeto.push_back(simbolo);
            }
        } else if (linha.find("estados:") == 0) {
            linha = linha.substr(8); // Remove "estados:"
            stringstream ss(linha);
            string estado;
            while (getline(ss, estado, ',')) {
                estados.push_back(estado);
            }
        } else if (linha.find("inicial:") == 0) {
            estado_inicial = linha.substr(8); // Remove "inicial:"
        } else if (linha.find("finais:") == 0) {
            linha = linha.substr(7); // Remove "finais:"
            stringstream ss(linha);
            string estado_final;
            while (getline(ss, estado_final, ',')) {
                estados_finais.push_back(estado_final);
            }
        } else if (linha.find("transicoes") == 0) {
            while (getline(file, linha)) {
                stringstream ss(linha);
                string q0, q1, simbolo;
                getline(ss, q0, ',');
                getline(ss, q1, ',');
                getline(ss, simbolo, ',');
                transicoes.emplace_back(q0, q1, simbolo);
            }
        }
    }
}

void automato::exibir() const {
    cout << "Alfabeto: ";
    for (const auto& simbolo : alfabeto) {
        cout << simbolo << " ";
    }
    cout << "\nEstados: ";
    for (const auto& estado : estados) {
        cout << estado << " ";
    }
    cout << "\nEstado Inicial: " << estado_inicial << endl;
    cout << "Estados Finais: ";
    for (const auto& estado_final : estados_finais) {
        cout << estado_final << " ";
    }
    cout << "\nTransicoes: \n";
    for (const auto& transicao : transicoes) {
        cout << get<0>(transicao) << " -> " << get<1>(transicao) << " [ " << get<2>(transicao) << " ]" << endl;
    }
}

bool automato::ehAFD() const {
    // Mapa para armazenar as transições já vistas de um estado com um determinado símbolo
    map<pair<string, string>, string> transicoes_vistas;

    // Itera sobre todas as transições do autômato
    for (int i = 0; i < transicoes.size(); i++) {
        string estado_atual = get<0>(transicoes[i]);
        string estado_destino = get<1>(transicoes[i]);
        string simbolo = get<2>(transicoes[i]);

        // Se a transição for um símbolo vazio (epsilon), o autômato é AFN
        if (simbolo == "") {
            return false;
        }

        // Verifica se já existe uma transição para o mesmo estado e símbolo
        pair<string, string> chave = make_pair(estado_atual, simbolo);
        if (transicoes_vistas.find(chave) != transicoes_vistas.end()) {
            // Já existe uma transição com o mesmo símbolo para o mesmo estado
            return false;
        }

        // Armazena a transição como vista
        transicoes_vistas[chave] = estado_destino;
    }


    // Se todas as transições forem únicas para cada estado e símbolo, é um AFD
    return true;
}

// Função auxiliar que retorna os estados de destino para um conjunto de estados e um símbolo
set<string> mover(const set<string>& conjunto_estados, const string& simbolo, const vector<tuple<string, string, string>>& transicoes) {
    set<string> novos_estados;
    for (int i = 0; i < conjunto_estados.size(); i++) {
        string estado = *next(conjunto_estados.begin(), i); // Acessa o estado atual usando iteradores
        for (int j = 0; j < transicoes.size(); j++) {
            if (get<0>(transicoes[j]) == estado && get<2>(transicoes[j]) == simbolo) {
                novos_estados.insert(get<1>(transicoes[j])); // Insere o novo estado
            }
        }
    }
    return novos_estados;
}


// Função para transformar um AFN em AFD
automato automato::transformarAFNparaAFD() const {
    automato afd;

    // Mapeamento de novos estados do AFD (como conjunto de estados do AFN) para nomes de estados
    map<set<string>, string> novos_estados_map;
    vector<tuple<string, string, string>> novas_transicoes;
    vector<string> novos_estados_finais;
    
    // Fila para processar novos estados
    queue<set<string>> fila_estados;

    // Nomear o estado inicial do AFD como o conjunto contendo o estado inicial do AFN
    set<string> estado_inicial_afd = { estado_inicial };
    fila_estados.push(estado_inicial_afd);
    novos_estados_map[estado_inicial_afd] = estado_inicial;  // Nomeia o conjunto inicial

    // Armazena o novo estado inicial
    afd.estado_inicial = estado_inicial;

    // Processa cada conjunto de estados na fila
    while (!fila_estados.empty()) {
        set<string> estados_atual = fila_estados.front();
        fila_estados.pop();
        
        // Verifica se o estado atual contém um estado final do AFN
        for (const auto& estado_final : estados_finais) {
            if (estados_atual.find(estado_final) != estados_atual.end()) {
                novos_estados_finais.push_back(novos_estados_map[estados_atual]);
                break;
            }
        }

        // Para cada símbolo no alfabeto, determina o novo conjunto de estados
        for (const auto& simbolo : alfabeto) {
            set<string> novos_estados = mover(estados_atual, simbolo, transicoes);

            if (!novos_estados.empty()) {
                // Verifica se o conjunto de estados já foi nomeado
                if (novos_estados_map.find(novos_estados) == novos_estados_map.end()) {
                    // Nomeia o novo conjunto de estados
                    string novo_estado_nome = "q" + to_string(novos_estados_map.size());
                    novos_estados_map[novos_estados] = novo_estado_nome;
                    fila_estados.push(novos_estados);
                }
                
                // Cria uma nova transição no AFD
                novas_transicoes.emplace_back(novos_estados_map[estados_atual], novos_estados_map[novos_estados], simbolo);
            }
        }
    }

    // Define os novos estados, transições e estados finais no AFD
    for (const auto& par : novos_estados_map) {
        afd.estados.push_back(par.second);
    }
    afd.transicoes = novas_transicoes;
    afd.estados_finais = novos_estados_finais;
    afd.alfabeto = alfabeto;

    return afd;
}
