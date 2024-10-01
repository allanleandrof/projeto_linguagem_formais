#include <iostream>
#include <fstream>
#include <string>
#include "automato.h"
#include "expressaoRegular.h"

using namespace std;

int main() {
    string arquivo;
    cout << "Digite o nome do arquivo: ";
    cin >> arquivo;

    // Detecta o tipo de formalismo baseado no conteúdo do arquivo
    ifstream file(arquivo);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << arquivo << endl;
        return 1;
    }

    string linha;
    bool isAutomato = false;
    bool isExpressao = false;

    // Lê as linhas do arquivo e verifica o tipo de formalismo
    while (getline(file, linha)) {
        if (linha.find("alfabeto:") == 0) {
            // Verifica se o arquivo é um AFD/AFN ou uma ER
            if (linha.find("transicoes") != string::npos) {
                isAutomato = true;
            } else if (linha.find("expressao:") != string::npos) {
                isExpressao = true;
            }
            break; // Pare de ler após encontrar a linha com "alfabeto:"
        }
    }

    if (isAutomato) {
        automato automato;
        automato.carregarAutomato(arquivo);
        cout << "Autômato carregado com sucesso!" << endl;
        automato.exibir();
    } else if (isExpressao) {
        expressaoRegular er;
        er.carregarER(arquivo);
        cout << "Expressão Regular carregada com sucesso!" << endl;
        er.exibir();
    } else {
        cerr << "Formato de arquivo inválido." << endl;
    }

    file.close(); // Fechar o arquivo
    return 0;
}
