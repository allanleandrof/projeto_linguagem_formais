#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "automato.h"
#include "expressaoRegular.h"

using namespace std;

int main() {
    string arquivo;
    cout << "Digite o nome do arquivo: ";
    cin >> arquivo;

    ifstream file(arquivo);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << arquivo << endl;
        return 1;
    }

    string linha;
    bool hasTransicoes = false;
    bool hasExpressao = false;

    // Ler todas as linhas do arquivo
    while (getline(file, linha)) {
        // Exibir cada linha para depuração
        cout << "Linha lida: " << linha << endl;
        if (linha.find("transicoes") != string::npos) {
            hasTransicoes = true;
        }
        if (linha.find("expressao:") != string::npos) {
            hasExpressao = true;
        }
    }

    // Baseado no conteúdo do arquivo, detecta o tipo
    if (hasTransicoes) {
        automato automato1;
        automato1.carregarAutomato(arquivo);
        cout << "Automato carregado com sucesso!" << endl;
        automato1.exibir();

        if (automato1.ehAFD()) {
        cout << "O automato eh um AFD." << endl;
        expressaoRegular er;
        er.converterAFDparaER(automato1);
        er.exibir();
        } else {
            cout << "O automato eh um AFN." << endl;
            automato converte = automato1.transformarAFNparaAFD();
            converte.exibir();
        }

    } else if (hasExpressao) {
        expressaoRegular er;
        er.carregarER(arquivo);
        cout << "Expressao Regular carregada com sucesso!" << endl;
        er.exibir();
    } else {
        cerr << "Formato de arquivo invalido." << endl;
    }

    file.close();
    return 0;
}