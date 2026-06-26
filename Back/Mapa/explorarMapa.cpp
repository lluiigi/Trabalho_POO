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

// --- Ajuste os caminhos conforme a estrutura de pastas do seu projeto ---
#include <winsock2.h>
// Linux / macOS
// #include <sys/socket.h> 

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
//  Estrutura interna para descrever um item sorteável de baú
// ================================================================
struct ItemBau {
    std::string nome;
    std::string categoria;  // "arma" ou "alimento"
    // Parâmetros extras usados na construção dos objetos concretos.
    // Adapte aos construtores reais de Arma e Alimento.
    int         valorNumerico; // dano (arma) ou cura (alimento)
    std::string tipoArma;      // "branca" ou "fogo" — ignorado para alimentos
};

// ================================================================
//  Helper: envia string + '\n' pelo socket
// ================================================================
static void enviarMensagem(int sock, const std::string& msg)
{
    std::string pacote = msg + "\n";
    ::send(sock, pacote.c_str(), static_cast<int>(pacote.size()), 0);
}

// ================================================================
//  Tabela de itens sorteáveis em baús
//
//  Adicione ou remova itens aqui livremente.
//  Os campos valorNumerico e tipoArma são usados nos construtores
//  de Arma e Alimento — ajuste se os seus construtores pedirem
//  parâmetros diferentes.
// ================================================================
static std::vector<ItemBau> tabelaItens()
{
    return {
        // [CORREÇÃO] Lista de armas reduzida para conter apenas Faca, Taco, Pistola e Escopeta
        // ---------- ARMAS ----------
        { "Faca",             "arma",     15, "Branca" },
        { "Taco de Beisebol", "arma",     20, "Branca" },
        { "Pistola",          "arma",     25, "Fogo"   },
        { "Escopeta",         "arma",     45, "Fogo"   },

        // ---------- ALIMENTOS / CONSUMÍVEIS ----------
        { "Enlatado",              "alimento", 20, "" },
        { "Barra de Cereal",       "alimento", 15, "" },
        { "Agua Potavel",          "alimento", 10, "" },
        { "Antibiotico",           "alimento", 35, "" },
        { "Kit Primeiros Socorros","alimento", 50, "" },
    };
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

    //  CASO C — Baú (15 %)    rolagem: 85–99
    // ==============================================================
    {
        // Sub-sorteio: 50% de chance de achar um Item (Arma/Comida) e 50% de chance de achar Munição
        int chanceMunicao = rand() % 2; 

        if (chanceMunicao == 0) {
            // Sorteia Munição
            std::string resp = "BAU_CONTEUDO:Caixa de Municao";
            enviarMensagem(clientSocket, resp);
            
            // Recarrega a arma que o jogador está segurando agora (se ele tiver uma)
            j->adicionarMunicaoArmaEquipada(10); 
            return resp;
        } 
        else {
            // Sorteia Arma ou Alimento da tabela existente
            std::vector<ItemBau> tabela = tabelaItens();
            int        idx  = rand() % static_cast<int>(tabela.size());
            ItemBau&   item = tabela[idx];

            // Protocolo → Python recebe "BAU_CONTEUDO:<nome>"
            std::string resp = "BAU_CONTEUDO:" + item.nome;
            enviarMensagem(clientSocket, resp);

           if (item.categoria == "arma") {
                // Pede a arma para a fábrica
                j->pegarItem(ItemFactory::criar_arma(item.nome));
            } else {
                // Pede o alimento para a fábrica
                j->pegarItem(ItemFactory::criar_alimento(item.nome));
            }

            return resp;
        }
    } // Fim da função explorarMapa
  
}