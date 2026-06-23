#ifndef ARMAS_HPP
#define ARMAS_HPP

#include <string>
#include <vector>
#include "itens.hpp"

using namespace std;

class Arma : public Item {
private:
    int dano;
    int municao;
    string categoria;
    float chance_aparecer;

public:
    Arma(string nome, string descricao, float peso, int dano, int municao,
         string categoria, float chance_aparecer);

    int getDano() const;
    int getMunicao() const;
    string getCategoria() const;
    float getChanceAparecer() const;
    string getTipo() const { return categoria; } // Adicionando getTipo() para compatibilidade
    void usar();
};

vector<Arma*> criarListaDeArmas();

#endif