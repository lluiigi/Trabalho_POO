#include <string>
#include "itens.hpp"
#ifndef armas_HPP
#define armas_HPP

class Arma : public Item {
private:
    int dano; // dano da arma, pode ser usado para calcular o dano das habilidades
    int municao; // quantidade de munição, relevante para armas de fogo
    std::string Tipo; // tipo da arma, como "branca" ou "de fogo"
public :
    Arma(std::string n, std::string desc, float p, int d, int m, std::string categoria);
    ~Arma();

    int getDano();
    int getMunicao();
    void usar(); // Método para usar a arma, pode reduzir a munição ou causar dano
    std::string getTipo();
};









#endif // armas_HPP