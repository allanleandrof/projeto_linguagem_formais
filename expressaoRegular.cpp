#include "expressaoRegular.h"
#include <algorithm>
#include <regex> 

expressaoRegular::expressaoRegular() {}

// Getters
vector<string> expressaoRegular::getAlfabeto() const {
    return alfabeto;
}

string expressaoRegular::getExpressao() const {
    return expressao;
}

// Setters
void expressaoRegular::setAlfabeto(const vector<string>& novoAlfabeto) {
    alfabeto = novoAlfabeto;
}

void expressaoRegular::setExpressao(const string& novaExpressao) {
    expressao = novaExpressao;
}

// Função para carregar uma ER de arquivo
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

// Exibe o alfabeto e a expressão regular
void expressaoRegular::exibir() const {
    cout << "Alfabeto: ";
    for (const auto& simbolo : alfabeto) {
        cout << simbolo << " ";
    }
    cout << "\nExpressao: " << expressao << endl;
}

void expressaoRegular::simplificaExpressao() {
    // Se a expressão estiver vazia, não há nada para simplificar
    if (expressao.empty()) {
        return;
    }

    // 1. Remove espaços desnecessários
    expressao.erase(remove(expressao.begin(), expressao.end(), ' '), expressao.end());

    // 2. Remove duplicatas em união
    set<string> termos;
    istringstream iss(expressao);
    string termo;
    string nova_expressao;

    while (getline(iss, termo, '+')) {
        termos.insert(termo);
    }

    for (const auto& t : termos) {
        if (!nova_expressao.empty()) {
            nova_expressao += "+";
        }
        nova_expressao += t;
    }
    expressao = nova_expressao;

    // 3. Combina sequências repetidas com Kleene
    // Exemplo: "a+a" -> "a*"
    regex kleene_regex("([a-zA-Z])\\1+");
    expressao = regex_replace(expressao, kleene_regex, "$1*");

    // 4. Simplificação de expressões como "a|b" ou "a+b"
    replace(expressao.begin(), expressao.end(), '+', '|');

    // 5. Remover expressões redundantes
    // Exemplo: "a|a" para "a"
    regex redundancia_regex("([a-zA-Z])\\|\\1");
    expressao = regex_replace(expressao, redundancia_regex, "$1");

    // 6. Remover parênteses desnecessários
    regex parentese_regex("\\(([^()]*)\\)");
    while (regex_search(expressao, parentese_regex)) {
        expressao = regex_replace(expressao, parentese_regex, "$1");
    }

    // 7. Remover sequências como "a*|a*" para "a*"
    regex redundancia_kleene_regex("([a-zA-Z]+\\*)\\|\\1");
    expressao = regex_replace(expressao, redundancia_kleene_regex, "$1");
}


// Função que converte um AFD em uma expressão regular
void expressaoRegular::converterAFDparaER(const automato& afd) {
    // Pega os dados do AFD
    vector<string> estados = afd.getEstados();
    vector<tuple<string, string, string>> transicoes = afd.getTransicoes();
    string estado_inicial = afd.getEstadoInicial();
    vector<string> estados_finais = afd.getEstadosFinais();

    setAlfabeto(afd.getAlfabeto());

    // Tabela de transições representando expressões regulares
    map<pair<string, string>, string> tabela;

    // Inicializa a tabela com as transições do AFD
    for (const auto& transicao : transicoes) {
        string origem = get<0>(transicao);
        string destino = get<1>(transicao);
        string simbolo = get<2>(transicao);

        // Adiciona a transição na tabela
        tabela[make_pair(origem, destino)] += simbolo;
    }

    // Eliminar estados intermediários
    for (const string& estado : estados) {
        if (estado != estado_inicial && find(estados_finais.begin(), estados_finais.end(), estado) == estados_finais.end()) {
            // Para cada par de estados (p, q), atualiza as transições p -> q usando o estado intermediário
            for (const string& p : estados) {
                for (const string& q : estados) {
                    pair<string, string> pq = make_pair(p, q);
                    pair<string, string> pp = make_pair(p, estado);
                    pair<string, string> qq = make_pair(estado, q);
                    pair<string, string> ee = make_pair(estado, estado);

                    string r_pq = tabela[pq];
                    string r_pp = tabela[pp];
                    string r_qq = tabela[qq];
                    string r_ee = tabela[ee];

                    // Fórmula da Eliminação de Estado: r(p,q) = r(p,q) + r(p,r)(r(r,r))*(r(r,q))
                    if (!r_pp.empty() && !r_qq.empty()) {
                        string novo_valor = r_pp;
                        if (!r_ee.empty()) {
                            novo_valor += "(" + r_ee + ")*";  // Inclui fechamento de Kleene para ciclos
                        }
                        novo_valor += r_qq;

                        if (!r_pq.empty()) {
                            novo_valor = r_pq + "+" + novo_valor;  // União das expressões
                        }
                        tabela[pq] = novo_valor;
                    }
                }
            }
            // Remove as transições para e do estado intermediário
            for (const string& p : estados) {
                tabela.erase(make_pair(p, estado));
                tabela.erase(make_pair(estado, p));
            }
        }
    }

    // Gera a expressão regular final, levando em conta ciclos
    string er_final;
    for (const string& final : estados_finais) {
        string transicao_final = tabela[make_pair(estado_inicial, final)];
        if (!er_final.empty() && !transicao_final.empty()) {
            er_final += "+" + transicao_final;  // União entre as transições
        } else {
            er_final = transicao_final;
        }

        // Se há um ciclo que retorna ao estado inicial, deve ser considerado
        string ciclo_inicial = tabela[make_pair(final, estado_inicial)];
        if (!ciclo_inicial.empty()) {
            er_final += "(" + ciclo_inicial + ")*";  // Fechamento de Kleene para ciclos
        }
    }

    // Define a expressão regular final
    expressao = er_final;

    // Corrige a expressão regular final para uma forma mais simples
    simplificaExpressao();  // Função adicional para simplificar a expressão
}