// ================================================================
//  explorarMapa.cpp  –  Função de exploração com RNG
//  RPG de Sobrevivência — backend C++ com sockets TCP
//
//  Usa EXCLUSIVAMENTE métodos que existem nos headers reais:
//
//  Jogador (jogador.hpp):
//    estaVivo()        → bool
//    pegarItem(Item*)  → void   (adiciona item à mochila interna)
//    ganhar_xp(int)    → void   (concede XP ao jogador)
//
//  Zumbi (zumbi.hpp):
//    estaVivo()        → bool
//    getTipo() const   → std::string
//    getXpRecompensa() → int
//
//  Probabilidades:
//     0–69  (70 %)  →  Nada
//    70–84  (15 %)  →  Zumbi  (ZumbiNormal / ZumbiTanque / ZumbiRapido — 1/3 cada)
//    85–99  (15 %)  →  Baú   (item aleatório das listas de armas/alimentos)
//
//  IMPORTANTE: inicialize a semente uma única vez no main():
//    srand(static_cast<unsigned int>(time(nullptr)));
// ================================================================

#include <winsock2.h>

#include "../Personagem/jogador.hpp"
#include "../Personagem/zumbi.hpp"
#include "../Itens/armas.hpp"
#include "../Itens/alimento.hpp"
#include "../Combate/combate.hpp"
#include "../fabricas.hpp"

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>    // rand()
#include <sstream>

// ================================================================
//  Helper: envia string + '\n' pelo socket
// ================================================================
static void enviarMensagem(int sock, const std::string& msg)
{
    std::string pacote = msg + "\n";
    ::send(sock, pacote.c_str(), static_cast<int>(pacote.size()), 0);
}

// ================================================================
//  explorarMapa
//
//  Parâmetros:
//    j            – ponteiro para o jogador atual
//    clientSocket – file descriptor do socket do cliente Python
//
//  Retorna a string de protocolo do evento ocorrido
//  (útil para log no servidor).
// ================================================================
std::string explorarMapa(Jogador* j, int clientSocket)
{
    int rolagem = rand() % 100;   // 0–99

    // ==============================================================
    //  CASO A — Nada (70 %)    rolagem: 0–69
    // ==============================================================
    if (rolagem < 70) {
        enviarMensagem(clientSocket, "EXPLORAR:NADA");
        return "EXPLORAR:NADA";
    }

    // ==============================================================
    //  CASO B — Zumbi (15 %)   rolagem: 70–84
    // ==============================================================
    if (rolagem < 85) {
        // Sorteia entre os 3 tipos definidos em zumbi.hpp
        int tipoIdx = rand() % 3;

        std::unique_ptr<Zumbi> zumbi;
        std::string            nomeZumbi;

        switch (tipoIdx) {
            case 0:
                zumbi     = std::make_unique<ZumbiNormal>();
                nomeZumbi = "ZumbiNormal";
                break;
            case 1:
                zumbi     = std::make_unique<ZumbiTanque>();
                nomeZumbi = "ZumbiTanque";
                break;
            default:
                zumbi     = std::make_unique<ZumbiRapido>();
                nomeZumbi = "ZumbiRapido";
                break;
        }

        // Avisa o Python: combate começando
        enviarMensagem(clientSocket, "EXPLORAR:ZUMBI:" + nomeZumbi);

        return "EXPLORAR:ZUMBI:" + nomeZumbi;
    }

    // ==============================================================
    //  CASO C — Baú (15 %)    rolagem: 85–99
    // ==============================================================
    {
        // Precisamos saber a classe do jogador. Usamos a sua própria classe auxiliar do combate!
        std::string classeJogador = CombateAux::lerClasse(j);

        if (classeJogador == "Atirador" || classeJogador == "ATIRADOR") {
            // ATIRADOR: Só acha Munição ou Comida
            int chance = rand() % 2; 
            if (chance == 0) {
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
            int chance = rand() % 2; 
            if (chance == 0) {
                // Sorteia entre Faca ou Taco
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
    } // Fim da função explorarMapa
}