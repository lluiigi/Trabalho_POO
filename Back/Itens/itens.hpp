#ifndef ITENS_HPP
#define ITENS_HPP

#include <string>


class Item { // classe item, serve com base para os outros "itens"
protected:  // esta no protected para que as classes derivadas possam acessar os atributos diretamente
    std::string nome;  // nome do item
    std::string descricao; // descricao do item (o que ele é)
    float peso; // seu peso para colocar na mochila 

public:
    Item(std::string n, std::string desc, float p); //contrutor da classe item, recebe nome, descricao e peso
    virtual ~Item(); // destrutor virtual para permitir a destruição correta de objetos derivados

    std::string getNome();
    float getPeso();
    virtual void exibirDetalhes() = 0; // função virtual pura para exibir detalhes do item
    // impedindo a crianção de novos itens, deixando a classe abstrata, e forçando as classes derivadas a implementarem essa função
   
}; 

#endif // ITENS_HPP