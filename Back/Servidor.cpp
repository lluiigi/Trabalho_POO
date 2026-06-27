#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Personagem/jogador.hpp" 
#include "Personagem/atirador.hpp"
#include "Personagem/zumbi.hpp"
#include "Combate/combate.hpp"
#include "fabricas.hpp"
using namespace std;

std::string explorarMapa(Jogador* j, int clientSocket);

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    srand(time(0)); 

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5000);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    cout << "=== BACK-END C++ LIGADO ===" << endl;
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    Jogador* personagemPrincipal = nullptr;
    char buffer[1024] = {0};


    recv(clientSocket, buffer, sizeof(buffer), 0);
    string mensagemRecebida(buffer);

    string nomeJogador = "Sobrevivente";
    string classeJogador = "Lutador";

    if (mensagemRecebida.find("INICIAR:") != string::npos) {
        int pos1 = mensagemRecebida.find(":");
        int pos2 = mensagemRecebida.find(":", pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos) {
            nomeJogador = mensagemRecebida.substr(pos1 + 1, pos2 - pos1 - 1);
            classeJogador = mensagemRecebida.substr(pos2 + 1);
            if (!classeJogador.empty() && classeJogador.back() == '\n') classeJogador.pop_back();
        }
    }

    personagemPrincipal = JogadorFactory::criar_jogador(classeJogador, nomeJogador);
    send(clientSocket, "Servidor C++: Personagem criado!\n", 33, 0);
    personagemPrincipal->exibirStatus();

    Zumbi zumbiNormal("Zumbi Normal", 100, 15, 20);
    Zumbi zumbiForte("Zumbi Forte", 50, 35, 50);
    Zumbi zumbiFraco("Zumbi Fraco", 200, 5, 10);
    Zumbi zumbiInimigo = zumbiNormal;
    Combate gerenciadorCombate;

    bool jogoRodando = true;
    while(jogoRodando) {
        char bufferComando[1024] = {0};
        int bytesRecebidos = recv(clientSocket, bufferComando, sizeof(bufferComando), 0);
        if (bytesRecebidos <= 0) break;

        string comando(bufferComando);
        
        if (comando.find("ANDAR") != string::npos || comando.find("EXPLORAR") != string::npos) {
            explorarMapa(personagemPrincipal, clientSocket);
        }
        else if (comando.find("ENCONTRO:INICIAR") != string::npos) {
            int sorteio = rand() % 3;
            if(sorteio == 0) zumbiInimigo = zumbiNormal;
            else if(sorteio == 1) zumbiInimigo = zumbiForte;
            else zumbiInimigo = zumbiFraco;
            send(clientSocket, ("HP_ZUMBI:" + to_string(zumbiInimigo.getVida()) + "\n").c_str(), 20, 0);
        }
       else if (comando.find("ATACAR") != string::npos) {
            gerenciadorCombate.Iniciar_Turno(personagemPrincipal, &zumbiInimigo);

            // Envia cada mensagem com um pequeno delay ou separadamente
            // Certifique-se de que cada uma termina com \n
            string msgHP_Jog = "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n";
            string msgHP_Zum = "HP_ZUMBI:" + to_string(zumbiInimigo.getVida()) + "\n";
            
            send(clientSocket, msgHP_Jog.c_str(), msgHP_Jog.length(), 0);
            send(clientSocket, msgHP_Zum.c_str(), msgHP_Zum.length(), 0);

            if (!zumbiInimigo.estaVivo()) {
                send(clientSocket, "BATALHA_FIM:VITORIA\n", 20, 0);
            } 
            else if (!personagemPrincipal->estaVivo()) {
                send(clientSocket, "BATALHA_FIM:DERROTA\n", 20, 0);
            } 
            else {
                send(clientSocket, "TEXTO:Turno encerrado! O zumbi contra-atacou.\n", 46, 0);
            }
        }
       else if (comando.find("MOCHILA:") != string::npos) {
            string nomeDoItem = comando.substr(8); 

            // ==============================================================
            // 1. CHAMA O SEU MÉTODO ORIENTADO A OBJETOS
            // ==============================================================
            // A sua função usarItemMochila já faz toda a mágica (cura, equipa arma)
            // e já devolve a string pronta (ex: "TEXTO:Voce consumiu Enlatado...\n")
            string msgTexto = personagemPrincipal->usarItemMochila(nomeDoItem);

            // ==============================================================
            // 2. AVISA O PYTHON DAS MUDANÇAS
            // ==============================================================
            // A) Remove o item da mochila gráfica
            string msgRemover = "REMOVER_ITEM:" + nomeDoItem + "\n";
            send(clientSocket, msgRemover.c_str(), msgRemover.length(), 0);
            
            // B) Envia o texto de feedback retornado pela sua função para destravar a tela
            send(clientSocket, msgTexto.c_str(), msgTexto.length(), 0);

            // C) Força a barra de vida verde do Python a atualizar
            string msgHP_Jog = "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n";
            send(clientSocket, msgHP_Jog.c_str(), msgHP_Jog.length(), 0);
        }
    }

    delete personagemPrincipal;
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}