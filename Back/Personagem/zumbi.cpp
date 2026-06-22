#include "zumbi.hpp"
#include <iostream>

//Implementando o construtor BASE
Zumbi::Zumbi(std::string tipo, int vida, int dano): tipo(tipo), vida(vida), dano(dano){}

//Quando Zumbi for atacado pelo jogador
void Zumbi::receberDano(int danoRecebido){
    vida = vida - danoRecebido;
    if(vida < 0){
        vida = 0; //vida não pode ser negativa
    }
    std::cout << "O" << tipo << "sofreu" << danoRecebido << "de dano! Vida restante: " << vida << "\n";

}

//Quando Zumbi for atacar
void Zumbi::atacar(){
    std::cout << "O" << tipo << "atacou e causou" << dano << "de dano!\n";
}

//Verifica se Zumbi ainda está vivo pra lutar
bool Zumbi::estaVivo(){
    return vida > 0;
}

//Retorno dos getters
std::string Zumbi::getTipo() const {return tipo;}
int Zumbi::getVida() const {return vida;}
int Zumbi::getDano() const {return dano;}
