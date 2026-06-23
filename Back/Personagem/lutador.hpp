#ifndef LUTADOR_HPP
#define LUTADOR_HPP

#include "jogador.hpp"

class Zumbi;

class Lutador : public Jogador {
public:
    Lutador(std::string n, std::string c, int niv, int hpm, int xp_inicial);
    Lutador(std::string n, std::string c);

    void atacar(Zumbi* alvo) override;
};

#endif