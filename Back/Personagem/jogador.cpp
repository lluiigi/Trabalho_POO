#include "jogador.hpp"
#include "zumbi.hpp"
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

// Construtor Completo (ÚNICA versão — duplicata removida)
Jogador::Jogador(string n, string c, int niv, int hpm, int xp_inicial) {
    nome = n;
    classe = c;
    hp_maximo = hpm;
    dano_base = 10;
    armaEquipada = nullptr;
    mochila = new Mochila(5);
    setNivel(niv);
    setHP(hpm);
    setXP(xp_inicial);
}

// Construtor Inicial
Jogador::Jogador(string n, string c) {
    nome = n;
    classe = c;
    hp_maximo = 100;
    nivel = 1;
    hp = 100;
    xp = 0;
    dano_base = 10;
    armaEquipada = nullptr;
    mochila = new Mochila(5);
}

Jogador::~Jogador() {
    delete mochila;
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
    cout << "Nome: " << nome << " (" << classe << ") | Nivel: " << nivel
         << " | HP: " << hp << "/" << hp_maximo
         << " | XP: " << xp << endl;
}

// --- FUNÇÕES DE COMBATE ---
void Jogador::equiparArma(Arma* novaArma) {
    armaEquipada = novaArma;
    if (armaEquipada != nullptr) {
        cout << "[Inventario]: " << nome << " equipou a arma!" << endl;
    }
}

// Função padrão. Lutador e Atirador sobrescrevem ela!
void Jogador::atacar(Zumbi* alvo) {
    if (!estaVivo()) return;

    int danoTotal = dano_base;
    if (armaEquipada != nullptr) {
        danoTotal += armaEquipada->getDano();
        armaEquipada->usar();
    }

    cout << nome << " atacou e causou " << danoTotal << " de dano!" << endl;
    alvo->receberDano(danoTotal);
}