#ifndef ITENS_HPP
#define ITENS_HPP

#include <string>


class Item { 
protected:
    std::string nome;
    std::string descricao;
    float peso;

public:
    Item(std::string n, std::string desc, float p);
    virtual ~Item();

    std::string getNome();
    float getPeso();
    virtual void exibirDetalhes();
}; 
#endif // ITENS_HPP