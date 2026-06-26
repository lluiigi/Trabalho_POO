#include "combate.hpp"
#include <iostream>

using namespace std;

Combate::Combate() {}// Construtor vazio, não há necessidade de inicializar nada aqui}

void Combate::Iniciar_Turno(Jogador* jogador, Zumbi* zumbi) {
    
    // verificando se algum dos personagens não existe = nulo
    if(jogador == nullptr || zumbi == nullptr) return;
    
    //inicio do combate
    cout << "\n--- Iniciando Turno de Combate ---" << endl;
    
    if (!jogador->estaVivo()) {
        cout << jogador->getNome() << " está morto e não pode atacar!" << endl;
        return;
    }
    if (!zumbi->estaVivo()) {
        cout << "O zumbi já está morto!" << endl;
        return;
    }

    // Jogador ataca primeiro
    jogador->atacar(zumbi);

   // Se o zumbi ainda estiver vivo, ele contra-ataca
    if (zumbi->estaVivo()) {
        zumbi->atacar(); // zumbi ataca
        
        // jogador recebe o dano do zumbi
        jogador->receber_dano(zumbi->getDano()); 
        
        cout << "\n[Status Atualizado]: ";
        jogador->exibirStatus();
    } else {
        // se o zumbi morreu jogador ganha xp
        cout << "\n*** O Zumbi foi derrotado! ***" << endl;
        jogador->ganhar_xp(zumbi->getXpRecompensa());
    }
}