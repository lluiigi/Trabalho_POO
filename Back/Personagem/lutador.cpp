#include "lutador.hpp"
#include <iostream>
using namespace std;

Lutador::Lutador(string n) : Jogador(n, "Lutador") {
    dano = 20; // Configuração específica desta classe
    hp_maximo = 150; // Lutador tem mais HP que a média
    hp = 150;
    habilidade_com_armasBrancas = 10; // Atributo exclusivo do Lutador
}


Lutador ::~Lutador() {
    cout << "[Destrutores]: Recursos especificos do Lutador limpos." << endl;
}
