#include <iostream>
#include "lutador.hpp"
#include "zumbi.hpp"

using namespace std;

Lutador::Lutador(string n, string c, int niv, int hpm, int xp_inicial)
    : Jogador(n, c, niv, hpm, xp_inicial) {}

Lutador::Lutador(string n, string c) : Jogador(n, c) {}

void Lutador::atacar(Zumbi* alvo) {
    if (!estaVivo()) return;

    int danoTotal = dano_base;

    if (armaEquipada != nullptr) {
        int danoArma = armaEquipada->getDano();

        if (armaEquipada->getTipo() == "Branca") {
            danoArma = static_cast<int>(danoArma * 1.5f);
            cout << nome << " executa um golpe brutal com afinidade em corpo a corpo! Ataque crítico!" << endl;
        }

        danoTotal += danoArma;
        armaEquipada->usar();
    }

    cout << nome << " atacou e causou " << danoTotal << " de dano!" << endl;
    alvo->receberDano(danoTotal);
}