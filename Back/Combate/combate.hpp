#pragma once
#ifndef COMBATE_HPP
#define COMBATE_HPP

// ================================================================
//  Combate.hpp  –  Módulo de Combate
//  RPG de Sobrevivência — backend C++ com sockets TCP
//
//  Consome os headers REAIS do projeto:
//    jogador.hpp  →  getVida(), receber_dano(), estaVivo(),
//                    classe (protected), dano_base (protected),
//                    armaEquipada (protected)
//    zumbi.hpp    →  getVida(), getDano(), receberDano(),
//                    estaVivo(), getTipo()
//
//  NÃO altera nenhuma dessas classes.
// ================================================================

#include <string>

// Forward declarations — evita incluir headers pesados aqui
class Jogador;
class Zumbi;

// ----------------------------------------------------------------
//  ResultadoTurno
//  Carrega todos os dados de um turno para o servidor poder
//  montar a mensagem de protocolo e tomar decisões.
// ----------------------------------------------------------------
struct ResultadoTurno {
    int  danoJogador;    // dano efetivo causado pelo jogador no zumbi
    int  danoZumbi;      // dano sofrido pelo jogador (0 se zumbi morreu antes)
    int  hpZumbiApos;    // HP do zumbi após o turno
    int  hpJogadorApos;  // HP do jogador após o turno
    bool zumbiMorreu;
    bool jogadorMorreu;
    bool bonusAplicado;  // true se o bônus de classe foi ativado neste turno
    std::string mensagem; // string de protocolo pronta para enviar ao Python
                          // formato:
                          // TURNO:JOGADOR_ATACOU:<d>:HP_ZUMBI:<h>:ZUMBI_ATACOU:<d>:HP_JOGADOR:<h>
                          // ou, se zumbi morreu no ataque do jogador:
                          // TURNO:JOGADOR_ATACOU:<d>:HP_ZUMBI:0
};

// ----------------------------------------------------------------
//  Classe Combate  (métodos todos estáticos — sem estado interno)
// ----------------------------------------------------------------
class Combate {
public:

    // ------------------------------------------------------------
    //  Iniciar_Turno
    //  Executa UM turno completo:
    //    1. Jogador ataca o zumbi (com bônus de classe se aplicável)
    //    2. Zumbi contra-ataca — SOMENTE se ainda estiver vivo
    //  Modifica hp do zumbi via receberDano() e hp do jogador via
    //  receber_dano().  Retorna ResultadoTurno preenchido.
    // ------------------------------------------------------------
    static ResultadoTurno Iniciar_Turno(Jogador* j, Zumbi* z);

    // ------------------------------------------------------------
    //  verificarVitoria
    //  Deve ser chamado APÓS Iniciar_Turno para checar o estado.
    //  Retorna:
    //    "VITORIA"  — zumbi não está mais vivo
    //    "DERROTA"  — jogador não está mais vivo
    //    "CONTINUA" — nenhum dos dois morreu ainda
    // ------------------------------------------------------------
    static std::string verificarVitoria(Jogador* j, Zumbi* z);

private:

    // ------------------------------------------------------------
    //  calcularDanoJogador
    //  Aplica o bônus de classe sobre o dano base do jogador:
    //    Lutador  + arma do tipo "branca"  →  dano_base × 1.5
    //    Atirador + arma do tipo "fogo"    →  dano_base × 1.5
    //    Qualquer outra combinação         →  dano_base × 1.0
    //
    //  Acessa classe e armaEquipada via herança protegida através
    //  de uma subclasse auxiliar interna (CombateAux), evitando
    //  modificar Jogador.
    // ------------------------------------------------------------
    static int calcularDanoJogador(Jogador* j);
};

// ================================================================
//  CombateAux  —  "Chave de acesso" aos membros protected de Jogador
//
//  Problema: classe e armaEquipada são protected em Jogador,
//  mas Combate não é subclasse e não pode acessá-los diretamente.
//
//  Solução sem tocar em Jogador: uma subclasse local que expõe
//  apenas o que o módulo de combate precisa, via static cast.
//  Nenhum objeto CombateAux é criado em runtime — apenas o
//  reinterpret do ponteiro para leitura.
// ================================================================
#include "../Personagem/jogador.hpp"   // ajuste o caminho conforme seu projeto
#include "../Itens/armas.hpp"          // ajuste o caminho conforme seu projeto

class CombateAux : public Jogador {
public:
    // Construtor nunca chamado — existe só para satisfazer o compilador
    CombateAux() : Jogador("", "") {}

    // Expõe os membros protected de Jogador para Combate
    static std::string lerClasse(Jogador* j) {
        return static_cast<CombateAux*>(j)->classe;
    }

    static int lerDanoBase(Jogador* j) {
        return static_cast<CombateAux*>(j)->dano_base;
    }

    static Arma* lerArmaEquipada(Jogador* j) {
        return static_cast<CombateAux*>(j)->armaEquipada;
    }
};

#endif // COMBATE_HPP