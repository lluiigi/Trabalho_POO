#include <iostream>
#include <string>

// Você vai dar include em todas as classes filhas que criar
#include "Jogador.hpp" 
#include "Atirador.hpp"
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

    // 3. A Magia do Polimorfismo (Ponteiros)
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

    // 4. Usando o personagem criado
    // ATENÇÃO: Como usamos ponteiro (*), substituímos o ponto (.) pela setinha (->)
    personagemPrincipal->exibirStatus(); 

    // Simulação rápida de jogo
    cout << "\nUm zumbi te atacou!" << endl;
    personagemPrincipal->receber_dano(25);
    personagemPrincipal->exibirStatus();

    // 5. Limpeza de Memória (Chama o Destrutor que configuramos antes)
    delete personagemPrincipal;

    return 0;
}