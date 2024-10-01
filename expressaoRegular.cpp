#include "expressaoRegular.h"
#include <iostream>
#include <fstream>
#include <sstream>

expressaoRegular::expressaoRegular() {}

void expressaoRegular::carregarER(const string& arquivo) {
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
        } else if (linha.find("expressao:") == 0) {
            expressao = linha.substr(10); // Remove "expressao:"
        }
    }
}

void expressaoRegular::exibir() const {
    cout << "Alfabeto: ";
    for (const auto& simbolo : alfabeto) {
        cout << simbolo << " ";
    }
    cout << "\nExpressão: " << expressao << endl;
}
