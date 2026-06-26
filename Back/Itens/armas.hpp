#ifndef ARMAS_HPP
#define ARMAS_HPP

#include <string>
#include <vector>
#include "itens.hpp"

using namespace std;

class Arma : public Item { // classe Arma, herda da classe Item
private:
    int dano;
    int municao;
    string categoria;
    float chance_aparecer;

public:
    Arma(string nome, string descricao, float peso, int dano, int municao, string categoria, float chance_aparecer); // Construtor da classe Arma, recebe nome, descricao, peso, dano, municao, categoria e chance de aparecer

    // Getters da classe Arma
    int getDano() const;
    int getMunicao() const;
    string getCategoria() const;
    float getChanceAparecer() const;
    string getTipo() const { return categoria; } // Adicionando getTipo() para compatibilidade
    void usar();
    void exibirDetalhes() override; // Implementa a função virtual pura da classe base Item
};

vector<Arma*> criarListaDeArmas();

#endif