#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") //vincula a biblioteca do winsock automaticamente

// Você vai dar include em todas as classes filhas que criar
#include "Personagem/jogador.hpp" 
#include "Personagem/atirador.hpp"
#include "Personagem/zumbi.hpp"
#include "Combate/combate.hpp"

using namespace std;

int main() {
    WSADATA wsaData;
    //inicia o uso da biblioteca winsock pra usar rede no Windows
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //cria a porta de comunicação do servidor
    // AF_INET = Protocolo IPv4 - SOCK_STREAM = Protocolo TCP (conexão estável e contínua)
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; //toda família de endereços vira IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; //aceita conexões de qualquer IP
    serverAddr.sin_port = htons(5000); //define a porta do servidor (será usada pelo Python)

    // Associa (bind) as configurações de IP e porta que criamos acima ao socket do servidor
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    //faz com que o servidor aguarde conexões, aceita até 1 conexão
    listen(serverSocket, 1);

    cout << "=== BACK-END C++ LIGADO ===" << endl;
    cout << "Aguardando conexao do Python..." << endl;

    //a execução do programa só começa quando o cliente (no Python) fizer a conexão
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    // Criamos uma variável "coringa" que pode guardar qualquer tipo de Jogador
    Jogador* personagemPrincipal = nullptr;

    char buffer[1024] = {0};

    //recebe a mensagem enviada pelo Python e armazena no buffer
    recv(clientSocket, buffer, sizeof(buffer), 0);
    
    string mensagemRecebida(buffer);
    cout << "[Recebido]: " << mensagemRecebida << endl;

    // Aqui usamos Polimorfismo, O ponteiro é do tipo Jogador, mas criamos um objeto da classe filha Atirador.
    personagemPrincipal = new Atirador("Luigi", "Atirador");

    // Por enquanto, vamos simular a resposta do C++:
    string resposta = "Servidor C++: Personagem criado com sucesso! HP: 100";
    
    // Enviando a resposta de volta para o Python
    send(clientSocket, resposta.c_str(), resposta.length(), 0);
    cout << "[Enviado]: " << resposta << endl;

    // 3. Usando o personagem criado
    personagemPrincipal->exibirStatus(); 


    // 4. Criando os tipos de inimigo

    //Notação: tipo de zumbi, vida, dano e XP que dá ao morrer
    Zumbi zumbiNormal("Zumbi Normal", 100, 15, 20);
    Zumbi zumbiForte("Zumbi Forte", 50, 35, 50);
    Zumbi zumbiFraco("Zumbi Fraco", 200, 5, 10);

    //zumbi padrão que servirá como inimigo
    Zumbi zumbiInimigo = zumbiNormal;

    //sorteia qual será o zumbi a ser enfrentado
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

    //instancia a classe Combate
    Combate gerenciadorCombate;

    // Simulação rápida de jogo
    cout << "\nUm " << zumbiInimigo.getTipo() << " apareceu!" << endl;

    bool fugiu = false;

    //o loop continua repetindo enquanto:
    //1. O zumbi estiver vivo
    //2. O jogador não fugir
    //3. O jogador estiver vivo
    while(zumbiInimigo.estaVivo() && !fugiu && personagemPrincipal->estaVivo()){
        cout << "\n SEU TURNO" << endl;
        cout << "1 - Atacar" << endl;
        cout << "2 - Ver Status" << endl;
        cout << "3 - Fugir" << endl;

        //lê a escolha digitada
        int acao;
        cin >> acao;

        if(acao == 1){
            //turno do jogador
            //se o jogador atacou, a classe Combate assume
            //ela pega o ponteiro do jogador e o endereço de memóriado zumbiInimigo
            //'Iniciar_Turno' vai calcular o dano do jogador e, se o zumbi sobreviver, o contra-ataque

            gerenciadorCombate.Iniciar_Turno(personagemPrincipal, &zumbiInimigo);

            if(!zumbiInimigo.estaVivo()){
            //o Iniciar_Turno já imprime as mensagens de morte e de ganho de XP
            break;
        }

        } else if (acao == 2){
            //mostra a vida atual de ambos sem avançar o turno
            cout << "\n SEUS STATUS " << endl;
            personagemPrincipal->exibirStatus();
            cout << "\n STATUS DO INIMIGO " << endl;
            cout << zumbiInimigo.getTipo() << ": Vida = " << zumbiInimigo.getVida() << "\n";
        } 
        else if(acao == 3){
            //marca o 'fugiu' como true. No próximo passo do while, ele sai do loop
            cout << "\n Você fugiu do " << zumbiInimigo.getTipo() << "!" << endl;
            fugiu = true;
        } 
        else{
            //se o jogador digitar 4 ou uma letra, ele perde o turno e o zumbi ataca de graça
            cout << "\n Ação inválida, você tropeçou e perdeu o turno de ataque!" << endl;
            cout << " TURNO DO INIMIGO " << endl;
            zumbiInimigo.atacar();
            personagemPrincipal->receber_dano(zumbiInimigo.getDano());
        }
    
    }

    cout << "FIM DA SIMULAÇÂO" << endl;

    // 5. Limpeza de Memória
    delete personagemPrincipal;

    //fecha o canal de comunicação com o Python
    closesocket(clientSocket);

    //fecha a porta 5000 do servidor
    closesocket(serverSocket);

    //desliga e limpa os recursos da biblioteca winsock
    WSACleanup();

    return 0;
}