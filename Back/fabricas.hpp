#ifndef FABRICAS_HPP
#define FABRICAS_HPP

#include <string>
#include "Personagem/jogador.hpp"
#include "Personagem/atirador.hpp"
#include "Personagem/lutador.hpp"
#include "Itens/armas.hpp"
#include "Itens/alimento.hpp"

// Fábrica de Personagens
class JogadorFactory {
public:
    static Jogador* criar_jogador(std::string classe, std::string nome);
};

// Fábrica de Itens
class ItemFactory {
public:
    static Arma* criar_arma(std::string tipo);
    static Alimento* criar_alimento(std::string tipo);
};

#endif