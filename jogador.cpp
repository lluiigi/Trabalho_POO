#include "jogador.hpp"
#include <iostream>

using namespace std;

void Jogador::setNivel(int valor) {
    if (valor < 0) nivel = 0;
    else nivel = valor;
}

void Jogador::setHP(int valor) {
    if (valor > hp_maximo) hp = hp_maximo;
    else if (valor < 0) hp = 0;
    else hp = valor;
}

void Jogador::setXP(int valor) {
    if (valor < 0) xp = 0;
    else xp = valor;
}

// Construtor Completo
Jogador::Jogador(string n, string c, int niv, int hpm, int xp_inicial) {
    nome = n;
    classe = c; // <-- Inicializa a classe
    hp_maximo = hpm;
    setNivel(niv);
    setHP(hpm);
    setXP(xp_inicial);
}

// Construtor Inicial (Mais usado na criação do personagem)
Jogador::Jogador(string n, string c) {
    nome = n;
    classe = c; // <-- Inicializa a classe
    hp_maximo = 100;
    nivel = 1;
    hp = 100;
    xp = 0;
}

Jogador::~Jogador() {
    cout << "[Destrutores]: Objeto do jogador '" << nome << "' foi liberado." << endl;
}

bool Jogador::estaVivo() {
    return hp > 0;
}

void Jogador::receber_dano(int quantidade) {
    if (quantidade > 0) setHP(hp - quantidade);
}

void Jogador::curar(int quantidade) {
    if (quantidade > 0) setHP(hp + quantidade);
}

void Jogador::ganhar_xp(int quantidade) {
    if (quantidade > 0) {
        setXP(xp + quantidade);
        cout << nome << " ganhou " << quantidade << " de XP!" << endl;
        if (xp >= 100) subir_nivel();
    }
}

void Jogador::subir_nivel() {
    nivel++;
    xp = xp - 100;
    hp_maximo += 20;
    hp = hp_maximo;
    cout << "--- LEVEL UP! " << nome << " alcancou o Nivel " << nivel << "! ---" << endl;
}

void Jogador::exibirStatus() {
    // Exibe a classe junto com as outras informações
    cout << "Nome: " << nome << " (" << classe << ") | Nivel: " << nivel 
         << " | HP: " << hp << "/" << hp_maximo 
         << " | XP: " << xp << endl;
}