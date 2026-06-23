#include "zumbi.hpp"
#include <iostream>

// ==========================================
// IMPLEMENTAÇÃO DA CLASSE BASE
// ==========================================

// 2. RESOLVIDO: O erro da linha 9 sumiu. Formatação certinha com chaves {}
Zumbi::Zumbi(std::string tipo, int vida, int dano, int xp_recompensa) 
    : tipo(tipo), vida(vida), dano(dano), xp_recompensa(xp_recompensa) {}

void Zumbi::receberDano(int danoRecebido){
    vida = vida - danoRecebido;
    if(vida < 0){
        vida = 0; 
    }
    std::cout << "O " << tipo << " sofreu " << danoRecebido << " de dano! Vida restante: " << vida << "\n";
}

void Zumbi::atacar(){
    std::cout << "O " << tipo << " atacou e causou " << dano << " de dano!\n";
}

bool Zumbi::estaVivo(){
    return vida > 0;
}

std::string Zumbi::getTipo() const {return tipo;}
int Zumbi::getVida() const {return vida;}
int Zumbi::getDano() const {return dano;}
int Zumbi::getXpRecompensa() const {return xp_recompensa;}


// ==========================================
// IMPLEMENTAÇÃO DOS ZUMBIS ESPECÍFICOS
// ==========================================

ZumbiNormal::ZumbiNormal() : Zumbi("Zumbi Normal", 50, 10, 20) {}

ZumbiTanque::ZumbiTanque() : Zumbi("Tanque", 150, 5, 50) {}

ZumbiRapido::ZumbiRapido() : Zumbi("Zumbi Rapido", 30, 20, 30) {}