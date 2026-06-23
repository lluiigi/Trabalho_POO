#include <iostream>
#include "atirador.hpp"
#include "zumbi.hpp"

using namespace std;

Atirador::Atirador(string n, string c, int niv, int hpm, int xp_inicial)
    : Jogador(n, c, niv, hpm, xp_inicial) {}

Atirador::Atirador(string n, string c) : Jogador(n, c) {}

void Atirador::atacar(Zumbi* alvo) {
    if (!estaVivo()) return;

    int danoTotal = dano_base;

    if (armaEquipada != nullptr) {
        int danoArma = armaEquipada->getDano();

        if (armaEquipada->getTipo() == "Fogo") {
            danoArma = static_cast<int>(danoArma * 1.5f);
            cout << nome << " realiza um tiro preciso com afinidade em armas de fogo! Acerto certeiro!" << endl;
        }

        danoTotal += danoArma;
        armaEquipada->usar();
    }

    cout << nome << " atacou e causou " << danoTotal << " de dano!" << endl;
    alvo->receberDano(danoTotal);
}