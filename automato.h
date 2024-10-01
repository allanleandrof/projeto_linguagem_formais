#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <vector>
#include <string>
#include <tuple>

using namespace std;

class automato {
private:
    vector<string> alfabeto;
    vector<string> estados;
    string estado_inicial;
    vector<string> estados_finais;
    vector<tuple<string, string, string>> transicoes; // q0, q1, a

public:
    automato();  // Construtor
    void carregarAutomato(const string& arquivo);  // Função para carregar um autômato
    void exibir() const;  // Função para exibir o autômato
};

#endif
