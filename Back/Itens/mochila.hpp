#ifndef MOCHILA_HPP
#define MOCHILA_HPP

#include <vector>
#include "itens.hpp"

//classe mochila que vai erdar de item 
class Mochila {
private:
    std::vector<Item*> itens; // vetor de ponteiros para itens, que vai armazenar os itens guardados na mochila
    int capacidade_maxima;

public:
    Mochila(int capacidade_maxima); // método construtor que inicializa a capacidade máxima da mochila

    bool guardarItem(Item* novoItem);
    void exibirItens();
};

#endif