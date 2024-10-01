#ifndef EXPRESSAOREGULAR_H
#define EXPRESSAOREGULAR_H

#include <vector>
#include <string>

using namespace std;

class expressaoRegular {
private:
    vector<string> alfabeto;
    string expressao;

public:
    expressaoRegular();  // Construtor
    void carregarER(const string& arquivo);  // Função para carregar a expressão regular
    void exibir() const;  // Função para exibir a expressão regular
};

#endif
