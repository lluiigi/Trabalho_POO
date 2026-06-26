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

// [NOVO] Protótipo da função para o servidor saber que ela existe no explorarMapa.cpp
std::string explorarMapa(Jogador* j, int clientSocket);

int main() {
    WSADATA wsaData;
    //inicia o uso da biblioteca winsock pra usar rede no Windows
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    srand(time(0)); // gerar numeros aleatórios diferentes a cada execução

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
    // Instancia a classe Combate
// Instancia a classe Combate
    Combate gerenciadorCombate;

    // A SEMENTE DO CAOS: Garante que os zumbis e danos sejam aleatórios
    srand(time(0)); 

    cout << "\n=== MOTOR DE JOGO INICIADO ===" << endl;

    // O servidor roda infinitamente esperando comandos
    bool jogoRodando = true;
    while(jogoRodando) {
        
        char bufferComando[1024] = {0};
        
        // O C++ congela aqui aguardando o clique no Pygame
        int bytesRecebidos = recv(clientSocket, bufferComando, sizeof(bufferComando), 0);
        
        if (bytesRecebidos <= 0) {
            cout << "Python desconectou. Encerrando..." << endl;
            break;
        }

        // Converte o que chegou para string
        string comando(bufferComando);
        cout << "[Pygame solicitou]: " << comando; // O comando já traz o '\n' do Python

        // ==========================================
        //        MÁQUINA DE ESTADOS DO JOGO
        // ==========================================

        // [NOVO] Adicionado o gatilho para chamar a função explorarMapa
        if (comando.find("ANDAR") != string::npos || comando.find("EXPLORAR") != string::npos) {
            explorarMapa(personagemPrincipal, clientSocket);
        }

        // 1. O Python avisa que um zumbi foi encontrado no mapa
        else if (comando.find("ENCONTRO:INICIAR") != string::npos) { // [CORREÇÃO] Adicionei 'else if' aqui para conectar com a condição de cima
            
            // Sorteia o zumbi
            int sorteio = rand() % 3;
            if(sorteio == 0) zumbiInimigo = zumbiNormal;
            else if(sorteio == 1) zumbiInimigo = zumbiForte;
            else zumbiInimigo = zumbiFraco;

            // Envia o HP real do zumbi sorteado para a tela do Python
            string respostaHP = "HP_ZUMBI:" + to_string(zumbiInimigo.getVida()) + "\n";
            send(clientSocket, respostaHP.c_str(), respostaHP.length(), 0);

            // Atualiza a caixa de texto
            string respostaTexto = "TEXTO:Um " + zumbiInimigo.getTipo() + " selvagem apareceu!\n";
            send(clientSocket, respostaTexto.c_str(), respostaTexto.length(), 0);
        }

        // 2. O jogador clicou no botão [1] ATACAR
        else if (comando.find("ATACAR") != string::npos) {
            
            // A nossa Fachada resolve a matemática do combate
            gerenciadorCombate.Iniciar_Turno(personagemPrincipal, &zumbiInimigo);

            // Manda o HP atualizado para as barras do Pygame diminuírem
            string msgHP_Jog = "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n";
            send(clientSocket, msgHP_Jog.c_str(), msgHP_Jog.length(), 0);

            string msgHP_Zum = "HP_ZUMBI:" + to_string(zumbiInimigo.getVida()) + "\n";
            send(clientSocket, msgHP_Zum.c_str(), msgHP_Zum.length(), 0);

            // Verifica o fim do combate para trocar a tela no Python
            if (!zumbiInimigo.estaVivo()) {
                string fim = "BATALHA_FIM:VITORIA\n";
                send(clientSocket, fim.c_str(), fim.length(), 0);
            } 
            else if (!personagemPrincipal->estaVivo()) {
                string fim = "BATALHA_FIM:DERROTA\n";
                send(clientSocket, fim.c_str(), fim.length(), 0);
            } 
            else {
                // Se ninguém morreu, a batalha continua
                string texto = "TEXTO:Turno encerrado! O zumbi contra-atacou.\n";
                send(clientSocket, texto.c_str(), texto.length(), 0);
            }
        }

        // [CORREÇÃO] Apaguei as verificações duplicadas de "ENCONTRO:INICIAR" e "ATACAR" que estavam repetidas aqui no seu código original

        else if (comando.find("MOCHILA:") != string::npos) {
            
            // 1. Extrai o nome do item (ex: "Kit Médico")
            int pos = comando.find(":");
            string nomeItem = comando.substr(pos + 1);
            if (!nomeItem.empty() && nomeItem.back() == '\n') {
                nomeItem.pop_back(); // Limpa a quebra de linha invisível
            }

            // 2. Cura o jogador (Vamos fixar em 20 de HP por enquanto)
            personagemPrincipal->curar(20); 

            // 3. Devolve a vida atualizada para a tela do Pygame
            string msgHP = "HP_JOGADOR:" + to_string(personagemPrincipal->getVida()) + "\n";
            send(clientSocket, msgHP.c_str(), msgHP.length(), 0);

            // 4. Manda a ordem para o Python DELETAR o item da lista
            string msgRemover = "REMOVER_ITEM:" + nomeItem + "\n";
            send(clientSocket, msgRemover.c_str(), msgRemover.length(), 0);

            // 5. Atualiza o chat da batalha
            string texto = "TEXTO:Voce usou " + nomeItem + " e recuperou 20 de HP!\n";
            send(clientSocket, texto.c_str(), texto.length(), 0);
        }
        // 3. O jogador clicou no botão [3] FUGIR
        else if (comando.find("FUGIR") != string::npos) {
            // Em um jogo real você faria o RNG da fuga aqui. 
            // Para simplificar, vamos deixar fugir direto e voltar para o mapa.
            string msgFuga = "BATALHA_FIM:FUGA\n";
            send(clientSocket, msgFuga.c_str(), msgFuga.length(), 0);
        }
    }   

// [CORREÇÃO] Arrumei a estrutura do final do código. Você fechou com "};", mas faltava limpar o socket e usar o "return 0;"
    delete personagemPrincipal; // Liberando memória que alocamos lá em cima
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}