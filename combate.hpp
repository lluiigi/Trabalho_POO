#include "jogador.hpp"
#ifndef COMBATE_HPP
#define COMBATE_HPP

class Combate : public Jogador {
private:
        int dano; // dano do Combate, pode ser usado para calcular o dano das habilidades
        int habilidae_com_armasBrancas; // Atributo exclusivo do Combate

public:
    Combate(std::string n);
    ~Combate();

    void usarHabilidade(); // Método exclusivo da classe Combate
};















#endif // COMBATE_HPP