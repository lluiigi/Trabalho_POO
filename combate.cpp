#include "combate.hpp"
#include <iostream>
using namespace std;

Combate::Combate(string n) : Jogador(n, "Combate") {
    dano = 15; // Configuração específica desta classe
    hp_maximo = 120; // Combate tem mais HP que a média
    hp = 120;
    habilidae_com_armasBrancas = 5; // 0atributo exclusivo do Combate
}
