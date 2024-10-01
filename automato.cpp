#include "Automato.h"
#include <iostream>
#include <fstream>
#include <sstream>

Automato::Automato() {}

void Automato::carregarAutomato(const string& arquivo) {
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

void Automato::exibir() const {
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
    cout << "\nTransições: \n";
    for (const auto& transicao : transicoes) {
        cout << get<0>(transicao) << " -> " << get<1>(transicao) << " [ " << get<2>(transicao) << " ]" << endl;
    }
}
