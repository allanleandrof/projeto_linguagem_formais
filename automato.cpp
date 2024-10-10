#include "automato.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

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
