#ifndef ATIRADOR_HPP
#define ATIRADOR_HPP

#include "jogador.hpp"

class Zumbi;

class Atirador : public Jogador {
public:
    Atirador(std::string n, std::string c, int niv, int hpm, int xp_inicial);
    Atirador(std::string n, std::string c);

    void atacar(Zumbi* alvo) override;
};

#endif