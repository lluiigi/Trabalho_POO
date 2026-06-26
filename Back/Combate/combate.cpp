// ================================================================
//  Combate.cpp  –  Implementação do módulo de combate
//  RPG de Sobrevivência — backend C++ com sockets TCP
//
//  Usa EXCLUSIVAMENTE métodos e atributos que existem nos
//  headers reais do projeto:
//
//  Jogador (jogador.hpp):
//    getVida() const   → int   (vida atual)
//    receber_dano(int) → void  (subtrai hp, já trata <= 0)
//    estaVivo()        → bool
//    [protected] classe       → std::string  (via CombateAux)
//    [protected] dano_base    → int          (via CombateAux)
//    [protected] armaEquipada → Arma*        (via CombateAux)
//
//  Zumbi (zumbi.hpp):
//    getVida() const   → int
//    getDano() const   → int
//    receberDano(int)  → void
//    estaVivo()        → bool
//    getTipo() const   → std::string  ("ZumbiNormal" | "ZumbiTanque" | "ZumbiRapido")
//
//  Arma (armas.hpp):
//    getTipo()         → std::string  (esperado: "branca" | "fogo")
// ================================================================

#include "combate.hpp"
#include "../Personagem/jogador.hpp"
#include "../Personagem/zumbi.hpp"
#include "../Itens/armas.hpp"
#include <algorithm>   // std::max
#include <sstream>     // std::ostringstream

// ================================================================
//  calcularDanoJogador  (privado)
//
//  Lê classe e arma via CombateAux (acesso a protected de Jogador).
//  Aplica multiplicador 1.5× quando classe + tipo de arma combinam.
// ================================================================
int Combate::calcularDanoJogador(Jogador* j)
{
    const float BONUS = 1.5f;

    int         danoBase = CombateAux::lerDanoBase(j);
    std::string classe   = CombateAux::lerClasse(j);
    Arma*       arma     = CombateAux::lerArmaEquipada(j);

    // Sem arma equipada — dano base, sem bônus
    if (arma == nullptr) {
        return danoBase;
    }

    std::string tipoArma = arma->getTipo(); // "branca" ou "fogo"

    bool aplicarBonus =
        (classe == "Lutador"  && tipoArma == "branca") ||
        (classe == "Atirador" && tipoArma == "fogo");

    if (aplicarBonus) {
        return static_cast<int>(danoBase * BONUS);
    }

    return danoBase;
}

// ================================================================
//  Iniciar_Turno
//
//  Sequência:
//    1. Calcula dano do jogador (com possível bônus de classe).
//    2. Aplica receberDano() no zumbi.
//    3. Se zumbi ainda viver: aplica receber_dano() no jogador.
//    4. Monta a string de protocolo e preenche ResultadoTurno.
//
//  String de protocolo gerada:
//    Zumbi sobrevive:
//      TURNO:JOGADOR_ATACOU:<d>:HP_ZUMBI:<h>:ZUMBI_ATACOU:<d>:HP_JOGADOR:<h>
//    Zumbi morreu no ataque do jogador:
//      TURNO:JOGADOR_ATACOU:<d>:HP_ZUMBI:0
// ================================================================
ResultadoTurno Combate::Iniciar_Turno(Jogador* j, Zumbi* z)
{
    ResultadoTurno resultado{};
    std::ostringstream msg;

    // ----------------------------------------------------------
    //  1. Jogador ataca o zumbi
    // ----------------------------------------------------------
    int danoJogador = calcularDanoJogador(j);

    // Verifica se o bônus foi aplicado para preencher o campo
    {
        std::string classe   = CombateAux::lerClasse(j);
        Arma*       arma     = CombateAux::lerArmaEquipada(j);
        std::string tipoArma = (arma != nullptr) ? arma->getTipo() : "";

        resultado.bonusAplicado =
            (classe == "Lutador"  && tipoArma == "branca") ||
            (classe == "Atirador" && tipoArma == "fogo");
    }

    z->receberDano(danoJogador);

    resultado.danoJogador  = danoJogador;
    resultado.hpZumbiApos  = z->getVida();
    resultado.zumbiMorreu  = !z->estaVivo();

    msg << "TURNO:"
        << "JOGADOR_ATACOU:" << danoJogador << ":"
        << "HP_ZUMBI:"       << resultado.hpZumbiApos;

    // ----------------------------------------------------------
    //  2. Contra-ataque do zumbi — somente se ainda vivo
    // ----------------------------------------------------------
    if (z->estaVivo()) {
        int danoZumbi = z->getDano();
        j->receber_dano(danoZumbi);

        resultado.danoZumbi     = danoZumbi;
        resultado.hpJogadorApos = j->getVida();
        resultado.jogadorMorreu = !j->estaVivo();

        msg << ":ZUMBI_ATACOU:" << danoZumbi
            << ":HP_JOGADOR:"   << resultado.hpJogadorApos;
    } else {
        // Zumbi morreu no ataque — jogador não toma dano neste turno
        resultado.danoZumbi     = 0;
        resultado.hpJogadorApos = j->getVida();
        resultado.jogadorMorreu = false;
    }

    resultado.mensagem = msg.str();
    return resultado;
}

// ================================================================
//  verificarVitoria
//
//  Chame APÓS Iniciar_Turno.
//  Retorna "VITORIA", "DERROTA" ou "CONTINUA".
// ================================================================
std::string Combate::verificarVitoria(Jogador* j, Zumbi* z)
{
    if (!z->estaVivo())  return "VITORIA";
    if (!j->estaVivo())  return "DERROTA";
    return "CONTINUA";
}