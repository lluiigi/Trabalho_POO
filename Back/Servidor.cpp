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
    string msgCriacao = "Servidor C++: Personagem criado!\n";
    send(clientSocket, msgCriacao.c_str(), msgCriacao.length(), 0);
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
            
            // [CORREÇÃO] Agora mandamos o HP_JOGADOR real para o Python não resetar a barra após uma fuga!
            string pacoteEncontro = 
                "HP_ZUMBI:" + to_string(zumbiInimigo.getVida()) + "\n" +
                "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n" +
                "TEXTO:Um " + zumbiInimigo.getTipo() + " apareceu!\n";
            
            send(clientSocket, pacoteEncontro.c_str(), pacoteEncontro.length(), 0);
        }
        else if (comando.find("ATACAR") != string::npos) {
            gerenciadorCombate.Iniciar_Turno(personagemPrincipal, &zumbiInimigo);

            // [CORREÇÃO] Empacota o status do combate numa única mensagem
            string pacoteCombate = 
                "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n" +
                "HP_ZUMBI:" + to_string(zumbiInimigo.getVida()) + "\n";
            
            if (!zumbiInimigo.estaVivo()) {
                pacoteCombate += "BATALHA_FIM:VITORIA\n";
            } 
            else if (!personagemPrincipal->estaVivo()) {
                pacoteCombate += "BATALHA_FIM:DERROTA\n";
            } 
            else {
                pacoteCombate += "TEXTO:Turno encerrado! O zumbi contra-atacou.\n";
            }
            send(clientSocket, pacoteCombate.c_str(), pacoteCombate.length(), 0);
        }
        else if (comando.find("MOCHILA:") != string::npos) {
            int pos = comando.find(":");
            string nomeItem = comando.substr(pos + 1);
            
            // [CORREÇÃO] Limpa sujeiras invisíveis de rede (como \r ou espaços) que impediam o uso do item
            nomeItem.erase(nomeItem.find_last_not_of(" \n\r\t") + 1);

            string textoAcao = personagemPrincipal->usarItemMochila(nomeItem);
            
            string pacoteMochila = 
                "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n" +
                "REMOVER_ITEM:" + nomeItem + "\n" +
                "TEXTO:" + textoAcao + "\n";
            
            send(clientSocket, pacoteMochila.c_str(), pacoteMochila.length(), 0);
        }
        // [CORREÇÃO] O bloco de FUGIR que estava faltando
        else if (comando.find("FUGIR") != string::npos) {
            string msgFuga = "BATALHA_FIM:FUGA\n";
            send(clientSocket, msgFuga.c_str(), msgFuga.length(), 0);
        }
    }

    delete personagemPrincipal;
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}