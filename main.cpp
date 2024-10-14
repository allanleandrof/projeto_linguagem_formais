#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "automato.h"
#include "expressaoRegular.h"

using namespace std;

int main() {
    string arquivo;
    automato automato1;
    expressaoRegular er;

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
        automato1.carregarAutomato(arquivo);
        cout << "Automato carregado com sucesso!" << endl;
        automato1.exibir();

        if (automato1.ehAFD()) {
        cout << "O automato eh um AFD." << endl;
        } else {
            cout << "O automato eh um AFN." << endl;
        }
    } else if (hasExpressao) {
        er.carregarER(arquivo);
        cout << "Expressao Regular carregada com sucesso!" << endl;
        er.exibir();
    } else {
        cerr << "Formato de arquivo invalido." << endl;
    }

    int num = 10;
    int sair;

    //menu de opções de conversão
    while (true) {
        automato converte;
        automato converte2;
        expressaoRegular er2;

        if (num == 0 || sair == 1) {
            cout << "Saindo do programa..." << endl;
            break; // Encerra o loop
        }

        cout << "Informe a conversao que deseja DIGITANDO o num correspondente: " << endl;
        cout << "1. AFN --> AFD " << endl;
        cout << "2. AFD --> ER " << endl;
        cout << "3. ER --> AFN " << endl;
        cout << "4. AFN --> ER " << endl;
        cout << "5. AFD --> AFN " << endl;
        cout << "6. ER --> AFD " << endl;
        cout << "0. SAIR. " << endl;

        cin >> num;

        switch (num) {
            case 1:
                converte = automato1.transformarAFNparaAFD();
                converte.exibir();
                cout << "AFN --> AFD" << endl;
                sair = 1;
                break;
            case 2:
                er2.converterAFDparaER(automato1);
                er2.exibir();
                cout << "AFD --> ER" << endl;
                sair = 1;
                break;
            case 3:
                converte = er.converterERparaAFN();
                converte.exibir();
                cout << "ER --> AFN" << endl;
                sair = 1;
                break;
            case 4:
                converte = automato1.transformarAFNparaAFD();
                er2.converterAFDparaER(converte);
                er2.exibir();
                cout << "AFN --> ER" << endl;
                sair = 1;
                break;
            case 5:
                er2.converterAFDparaER(automato1);
                converte = er.converterERparaAFN();
                converte.exibir();
                cout << "AFD --> AFN" << endl;
                sair = 1;
                break;
            case 6:
                converte2 = er.converterERparaAFN();
                converte = converte2.transformarAFNparaAFD();
                converte.exibir();
                cout << "ER --> AFD" << endl;
                sair = 1;
                break;
            default:
                cout << "Numero invalido! Por favor, digite um numero entre 1 e 7." << endl;
                break;
        }
    }


    file.close();
    return 0;
}