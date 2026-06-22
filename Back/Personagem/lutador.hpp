#include "jogador.hpp"
#ifndef LUTADOR_HPP
#define LUTADOR_HPP

class Lutador : public Jogador {
private:
        int dano; // dano do Lutador, pode ser usado para calcular o dano das habilidades
        int habilidade_com_armasBrancas; // Atributo exclusivo do Lutador

public:
    Lutador(std::string n);
    ~Lutador();

    void usarHabilidade(); // Método exclusivo da classe Lutador
};















#endif //LUTADOR_HPP