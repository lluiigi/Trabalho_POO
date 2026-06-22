#include "atirador.hpp"
#include <iostream>

using namespace std;

// Construtor: Repassa os dados para o construtor do Jogador e define a munição inicial
Atirador::Atirador(string n) : Jogador( n, "Atirador") {
    municao = 30; // Configuração específica desta classe
    hp_maximo = 90; // Atiradores podem ter um pouco menos de HP que a média
    hp = 90;
}

Atirador::~Atirador() {
    cout << "[Destrutores]: Recursos especificos do Atirador limpos." << endl;
}

void Atirador::usarHabilidade() {
    if (municao > 0) {
        municao -= 3;
        cout << nome << " disparou uma rajada! Municao restante: " << municao << endl;
    } else {
        cout << nome << " esta sem municao!" << endl;
    }
}