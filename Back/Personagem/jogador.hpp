#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include <string>
#include "itens/armas.hpp"
#include "mochila.hpp"

class Zumbi;

class Jogador {
protected:
    std::string nome;
    std::string classe;
    int nivel;
    int hp;
    int hp_maximo;
    int xp;

    int dano_base;
    Arma* armaEquipada;
    Mochila* mochila;   // <-- nome padronizado

    void setNivel(int valor);
    void setHP(int valor);
    void setXP(int valor);

public:
    Jogador(std::string n, std::string c, int niv, int hpm, int xp_inicial);
    Jogador(std::string n, std::string c);
    virtual ~Jogador();

    void equiparArma(Arma* novaArma);
    virtual void atacar(Zumbi* alvo);

    void receber_dano(int quantidade);
    void curar(int quantidade);
    void ganhar_xp(int quantidade);
    void subir_nivel();
    void exibirStatus();
    bool estaVivo();
};

#endif // JOGADOR_HPP