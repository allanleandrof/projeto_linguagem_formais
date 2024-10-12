#ifndef EXPRESSAOREGULAR_H
#define EXPRESSAOREGULAR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include "automato.h"

using namespace std;

class expressaoRegular {
private:
    vector<string> alfabeto;
    string expressao;

public:
    expressaoRegular();  // Construtor
    
    // Getters e Setters
    vector<string> getAlfabeto() const;
    void setAlfabeto(const vector<string>& novoAlfabeto);

    string getExpressao() const;
    void setExpressao(const string& novaExpressao);

    // Funções principais
    void carregarER(const string& arquivo);  // Função para carregar a expressão regular
    void exibir() const;  // Função para exibir a expressão regular
    void converterAFDparaER(const automato& afd);  // Função para converter AFD para ER
    void simplificaExpressao();  // Função para converter AFD para ER
};

#endif
