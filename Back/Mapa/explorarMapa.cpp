#include <winsock2.h>
#include "../Personagem/jogador.hpp"
#include "../Personagem/atirador.hpp" // Fundamental para o dynamic_cast funcionar
#include "../Personagem/zumbi.hpp"
#include "../Itens/armas.hpp"
#include "../Itens/alimento.hpp"
#include "../fabricas.hpp"

#include <string>
#include <cstdlib>

// ================================================================
//  Helper: envia string + '\n' pelo socket
// ================================================================
static void enviarMensagem(int sock, const std::string& msg) {
    std::string pacote = msg + "\n";
    ::send(sock, pacote.c_str(), static_cast<int>(pacote.size()), 0);
}

// ================================================================
//  explorarMapa
// ================================================================
std::string explorarMapa(Jogador* j, int clientSocket) {
    int rolagem = rand() % 100;   // 0–99

    // ==============================================================
    //  CASO A — Nada (70 %)
    // ==============================================================
    if (rolagem < 70) {
        enviarMensagem(clientSocket, "EXPLORAR:NADA");
        return "EXPLORAR:NADA";
    }
    
    // ==============================================================
    //  CASO B — Zumbi (15 %)
    // ==============================================================
    else if (rolagem < 85) {
        int tipoIdx = rand() % 3;
        std::string nomeZumbi;

        // Ajustado para bater com os nomes que o front-end e o servidor esperam
        if (tipoIdx == 0) nomeZumbi = "Zumbi Normal";
        else if (tipoIdx == 1) nomeZumbi = "Zumbi Forte";
        else nomeZumbi = "Zumbi Fraco";

        enviarMensagem(clientSocket, "EXPLORAR:ZUMBI:" + nomeZumbi);
        return "EXPLORAR:ZUMBI:" + nomeZumbi;
    }
    
    // ==============================================================
    //  CASO C — Baú (15 %)
    // ==============================================================
    else {
        // Tenta converter o ponteiro base (Jogador) para a classe derivada (Atirador)
        Atirador* atirador = dynamic_cast<Atirador*>(j);

        // Se atirador NÃO for nulo, significa que a conversão deu certo!
        if (atirador != nullptr) {
            // ATIRADOR: Só acha Munição ou Comida
            if (rand() % 2 == 0) {
                std::string resp = "BAU_CONTEUDO:Caixa de Municao";
                enviarMensagem(clientSocket, resp);
                
                j->adicionarMunicaoArmaEquipada(10); 
                return resp;
            } else {
                std::string resp = "BAU_CONTEUDO:Enlatado";
                enviarMensagem(clientSocket, resp);
                j->pegarItem(ItemFactory::criar_alimento("Enlatado"));
                return resp;
            }
        } 
        else {
            // LUTADOR: Só acha Arma Branca ou Comida
            if (rand() % 2 == 0) {
                std::string armaSorteada = (rand() % 2 == 0) ? "Faca" : "Taco de Beisebol";
                std::string resp = "BAU_CONTEUDO:" + armaSorteada;
                enviarMensagem(clientSocket, resp);
                
                j->pegarItem(ItemFactory::criar_arma(armaSorteada));
                return resp;
            } else {
                std::string resp = "BAU_CONTEUDO:Antibiotico";
                enviarMensagem(clientSocket, resp);
                j->pegarItem(ItemFactory::criar_alimento("Antibiotico"));
                return resp;
            }
        }
    } 
}