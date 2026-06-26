#ifndef COMBATE_HPP
#define COMBATE_HPP
#include "../Personagem/jogador.hpp"
#include "../Personagem/zumbi.hpp"

//classe para "INICIAR" o combate entre jogador e zumbi, gerenciando os turnos e a interação entre eles
class Combate {
public:
    Combate(); // construtor da classe combate, não recebe parametros

    void Iniciar_Turno(Jogador* jogador, Zumbi* zumbi); // void para iniciar um turno do jogo
    // usado ponteiro de jogador e zumbi para poder acessar os atributos e metodos deles, e poder modificar eles durante o combate
};
















#endif // COMBATE_HPP