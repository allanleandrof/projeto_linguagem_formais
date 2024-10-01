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
    string linha;
    if (getline(file, linha)) {
        if (linha.find("alfabeto:") == 0 && linha.find("transicoes") != string::npos) {
            // Caso seja um AFD/AFN
            automato automato;
            automato.carregarAutomato(arquivo);
            cout << "Autômato carregado com sucesso!" << endl;
            automato.exibir();
        } else if (linha.find("alfabeto:") == 0 && linha.find("expressao:") != string::npos) {
            // Caso seja uma Expressão Regular
            expressaoRegular er;
            er.carregarER(arquivo);
            cout << "Expressão Regular carregada com sucesso!" << endl;
            er.exibir();
        } else {
            cerr << "Formato de arquivo inválido." << endl;
        }
    }

    return 0;
}
