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

    // Getters
    vector<string> getAlfabeto() const;
    vector<string> getEstados() const;
    string getEstadoInicial() const;
    vector<string> getEstadosFinais() const;
    vector<tuple<string, string, string>> getTransicoes() const;

    // Setters
    void setAlfabeto(const vector<string>& novoAlfabeto);
    void setEstados(const vector<string>& novosEstados);
    void setEstadoInicial(const string& novoEstadoInicial);
    void setEstadosFinais(const vector<string>& novosEstadosFinais);
    void setTransicoes(const vector<tuple<string, string, string>>& novasTransicoes);

    // Funções auxiliares para manipulação do autômato
    void adicionarEstado(const string& estado);
    void adicionarEstadoFinal(const string& estado);
    void adicionarTransicao(const string& origem, const string& destino, const string& simbolo);
    void mesclarAutomato(const automato& outro);

    void carregarAutomato(const string& arquivo);  // Função para carregar um autômato
    void exibir() const;  // Função para exibir o autômato
    bool ehAFD() const;
    automato transformarAFNparaAFD() const;
};

#endif