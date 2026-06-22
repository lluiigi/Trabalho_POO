#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

// Você vai dar include em todas as classes filhas que criar
#include "Jogador.hpp" 
#include "Atirador.hpp"
#include "zumbi.hpp"
// #include "Combate.hpp" // (Quando você criar o arquivo Combate.hpp)

using namespace std;

int main() {
    string nomeDigitado;
    int escolhaClasse = 0;

    cout << "========================================" << endl;
    cout << "       BEM-VINDO AO RPG DE ZUMBI        " << endl;
    cout << "========================================" << endl;

    // 1. Capturando o Nome do Usuário
    cout << "\nDigite o nome do seu sobrevivente: ";
    getline(cin, nomeDigitado); // Usamos getline no lugar de cin para permitir nomes compostos (ex: "Jota Silva")

    // 2. Menu de Escolha de Classe
    cout << "\nEscolha sua especializacao de sobrevivencia:" << endl;
    cout << "1 - Atirador (Foco em armas de fogo)" << endl;
    cout << "2 - Combate (Foco em forca bruta corporal)" << endl;
    cout << "Sua escolha: ";
    cin >> escolhaClasse;

    // Criamos uma variável "coringa" que pode guardar qualquer tipo de Jogador
    Jogador* personagemPrincipal = nullptr;

    // Instanciamos a classe correta com base no número digitado
    if (escolhaClasse == 1) {
        personagemPrincipal = new Atirador(nomeDigitado);
        cout << "\nClasse Atirador selecionada!" << endl;
        
    } else if (escolhaClasse == 2) {
        // personagemPrincipal = new Combate(nomeDigitado); 
        cout << "\nClasse Combate selecionada! (Usando Atirador provisoriamente)" << endl;
        personagemPrincipal = new Atirador(nomeDigitado); // Provisório até você criar o Combate.cpp
        
    } else {
        cout << "\nOpcao invalida. Criando Atirador por padrao." << endl;
        personagemPrincipal = new Atirador(nomeDigitado);
    }

    cout << "----------------------------------------" << endl;

    // 3. Usando o personagem criado
    personagemPrincipal->exibirStatus(); 


    // 4. Criando os tipos de inimigo

    //Notação: tipo de zumbi, vida e dano
    Zumbi zumbiNormal("Zumbi Normal", 100, 15);
    Zumbi zumbiForte("Zumbi Forte", 50, 35);
    Zumbi zumbiFraco("Zumbi Fraco", 200, 5);

    Zumbi zumbiInimigo = zumbiNormal;

    int sorteio = rand() % 3; // sorteia um número: 0, 1 ou 2

    if(sorteio == 0){
        zumbiInimigo = zumbiNormal;
    }
    else if(sorteio == 1){
        zumbiInimigo = zumbiForte;
    }
    else{
        zumbiInimigo = zumbiFraco;
    }

    // Simulação rápida de jogo
    cout << "\nUm " << zumbiInimigo.getTipo() << " apareceu!" << endl;

    bool fugiu = false;

    while(zumbiInimigo.estaVivo() && !fugiu){
        
    }

    personagemPrincipal->receber_dano(25);
    personagemPrincipal->exibirStatus();

    // 5. Limpeza de Memória
    delete personagemPrincipal;

    return 0;
}