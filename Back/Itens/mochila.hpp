#ifndef MOCHILA_HPP
#define MOCHILA_HPP

#include <vector>
#include "itens.hpp"

class Mochila {
private:
    std::vector<Item*> itens;
    int capacidade_maxima;

public:
    Mochila(int capacidade_maxima);

    bool guardarItem(Item* novoItem);
    void exibirItens();
};

#endif