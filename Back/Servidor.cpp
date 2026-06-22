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
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5000);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    cout << "=== BACK-END C++ LIGADO ===" << endl;
    cout << "Aguardando conexao do Python..." << endl;

    // Criamos uma variável "coringa" que pode guardar qualquer tipo de Jogador
    Jogador* personagemPrincipal = nullptr;

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    
    string mensagemRecebida(buffer);
    cout << "[Recebido]: " << mensagemRecebida << endl;

    // --- AQUI ENTRA A SUA LÓGICA DE POO ---
    // Exemplo: se recebemos "Criar_Personagem:Luigi"
    // Atirador* p1 = new Atirador("Luigi");
    // string hp_atual = to_string(p1->getHP());
    
    // Por enquanto, vamos simular a resposta do C++:
    string resposta = "Servidor C++: Personagem criado com sucesso! HP: 100";
    
    // Enviando a resposta de volta para o Python
    send(clientSocket, resposta.c_str(), resposta.length(), 0);
    cout << "[Enviado]: " << resposta << endl;

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
        cout << "\n SEU TURNO" << endl;
        cout << "1 - Atacar" << endl;
        cout << "2 - Ver Status" << endl;
        cout << "3 - Fugir" << endl;

        int acao;
        cin >> acao;

        if(acao == 1){
            //turno do jogador
            int danoDoJogador = 30;
            cout << "\n Você atacou o " << zumbiInimigo.getTipo() << endl;
            zumbiInimigo.receberDano(danoDoJogador);

            if(!zumbiInimigo.estaVivo()){
            cout << "\n Você derrotou o " << zumbiInimigo.getTipo() << endl;
            break;
        }

            //turno do zumbi
        cout << "\n TURNO DO INIMIGO" << endl;
        zumbiInimigo.atacar();
        personagemPrincipal->receber_dano(zumbiInimigo.getDano());

        } else if (acao == 2){
            cout << "\n SEUS STATUS " << endl;
            personagemPrincipal->exibirStatus();
            cout << "\n STATUS DO INIMIGO " << endl;
            cout << zumbiInimigo.getTipo() << ": Vida = " << zumbiInimigo.getVida() << "\n";
        } else if(acao == 3){
            cout << "\n Você fugiu do " << zumbiInimigo.getTipo() << "!" << endl;
            fugiu = true;
        } else{
            cout << "\n Ação inválida, você tropeçou e perdeu o turno de ataque!" << endl;
            cout << " TURNO DO INIMIGO " << endl;
            zumbiInimigo.atacar();
            personagemPrincipal->receber_dano(zumbiInimigo.getDano());
        }
    
    }

    cout << "FIM DA SIMULAÇÂO" << endl;

    // 5. Limpeza de Memória
    delete personagemPrincipal;

    return 0;
}