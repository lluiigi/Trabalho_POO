#include <iostream>
#include <string>
#include <winsock2.h>
// Inclua suas classes aqui:
// #include "jogador.hpp"
// #include "atirador.hpp"

#pragma comment(lib, "ws2_32.lib") 

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

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

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

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}