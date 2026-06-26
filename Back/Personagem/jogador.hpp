#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include <string>
#include "itens/armas.hpp" // chamna o arquivo armas.hpp para poder usar a classe Arma
#include "mochila.hpp" // chama o arquivo mochila.hpp para poder usar a classe Mochila
#include "Itens/alimento.hpp" // chama o arquivo alimento.hpp para poder usar a classe Alimento

class Zumbi;

class Jogador {
protected:
    std::string nome;  // nome do jogador
    std::string classe; // classe que pertence o jogador
    int nivel; // nivel do jogador
    int hp; // vida atual do jogador
    int hp_maximo; // vida máxima do jogador
    int xp; // experiencia do jogador

    int dano_base; // dano base do jogador, sem armas
    Arma* armaEquipada; // dano do jogador com arma equipada, se tiver, se não, é 0
    Mochila* mochila;  // "iventario" do jogador, onde ele guarda os itens que encontra


    void setNivel(int valor); // void para setar o nivel do jogador
    void setHP(int valor); // void para setar a vida do jogador
    void setXP(int valor); // void para setar a experiencia do jogador

public:
    Jogador(std::string n, std::string c, int niv, int hpm, int xp_inicial); // Construtor completo, recebe nome, classe, nivel, vida máxima e experiencia inicial
    Jogador(std::string n, std::string c); // Construtor inicial, recebe apenas nome e classe, o resto é setado com valores padrão
    virtual ~Jogador(); // destrutor virtual para permitir a destruição correta de objetos derivados
  
    std::string getNome(); // Getter para obter o nome do jogador
    // são os as funçoes de status do jogador :

    void equiparArma(Arma* novaArma); //void para equipar uma arma, recebe um ponteiro para a arma que será equipada
    virtual void atacar(Zumbi* alvo);//void para atacar um zumbi, recebe um ponteiro para o zumbi que será atacado, é virtual para que as classes derivadas possam sobrescrever ela

    void receber_dano(int quantidade); // void para receber dano, recebe a quantidade de dano que será subtraida da vida do jogador
    void curar(int quantidade); // void para curar o jogador, recebe a quantidade de vida que será adicionada a vida do jogador
    void ganhar_xp(int quantidade); // void para ganhar experiencia, recebe a quantidade de experiencia que será adicionada a experiencia do jogador
    void subir_nivel(); // void para subir de nivel, aumenta o nivel do jogador, aumenta a vida máxima e seta a vida atual para a vida máxima, reseta a experiencia para 0
    void exibirStatus(); // void para exibir o status do jogador, exibe nome, classe, nivel, vida atual/vida máxima e experiencia atual
    bool estaVivo(); // bool para verificar se o jogador está vivo, retorna true se a vida for maior que 0, false caso contrário
    void pegarItem(Item* novoItem); // void para pegar um item, recebe um ponteiro para o item que será pego, tenta guardar na mochila, se não conseguir, o item é perdido
    void verMochila(); // void para ver o conteúdo da mochila, chama a função exibirItens() da classe Mochila
    void comer(Alimento* comida); // void para comer um alimento, recebe um ponteiro para o alimento que será comido, aumenta a vida do jogador de acordo com a quantidade de cura do alimento, e remove o item da mochila

   
};

#endif // JOGADOR_HPP