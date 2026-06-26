#ifndef ALIMENTO_HPP
#define ALIMENTO_HPP
#include <string>
#include "itens.hpp"

class Alimento : public Item {
private:
    int cura_hp; // Quantidade de HP que o alimento cura
public:
   Alimento(std::string nome, std::string descricao, float peso, int recuperacao_hp); // Construtor
    ~Alimento(); // destrutor

    int getCuraHP(); // Getter para obter a quantidade de HP que o alimento cura

    void exibirDetalhes() override; // implemneta a funçao virtual pura da classe base Item, exibe detalhes do alimento
};
#endif // ALIMENTO_HPP