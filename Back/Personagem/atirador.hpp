#ifndef ATIRADOR_HPP
#define ATIRADOR_HPP

#include "jogador.hpp" // Precisa incluir o topo da classe mãe

class Atirador : public Jogador {
private:
    int municao; // Atributo exclusivo do Atirador
    int dano; // Dano base do Atirador, pode ser usado para calcular o dano das habilidades
public:
    // O construtor do Atirador precisa chamar o construtor do Jogador
    Atirador(std::string n);
    ~Atirador();

    // Método exclusivo da classe Atirador
    void usarHabilidade(); 
};

#endif // ATIRADOR_HPP