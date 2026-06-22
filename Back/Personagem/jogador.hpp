#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include <string>

class Jogador {
protected: // Protected para que os filhos possam acessar diretamente
    std::string nome; // nome do personagem
    std::string classe; // classe do personagem
    int nivel; // nível do personagem
    int hp; // pontos de vida atuais
    int hp_maximo; // pontos de vida máximos, usado para limitar a cura e o dano
    int xp; // pontos de experiência, usado para subir de nível

    void setNivel(int valor); //  garantir que o nível nunca seja negativo
    void setHP(int valor); // garantir que o HP nunca seja negativo ou maior que o máximo
    void setXP(int valor); // garantir que o XP nunca seja negativo

public:
   //contrutores que vao ser usados para receber os dados do personagem e criar o objeto
    Jogador(std::string n, std::string c, int niv, int hpm, int xp_inicial);
    Jogador(std::string n, std::string c); 
    
    // destrutor
    virtual ~Jogador();

    // Métodos
    void receber_dano(int quantidade); //void para recebre dano
    void curar(int quantidade); // void para curar o personagem
    void ganhar_xp(int quantidade); // void para ganhar xp e subir de nível quando atingir 100
    void subir_nivel(); // void para subir de nível, aumenta o nível e reseta o XP
    void exibirStatus(); // void para exibir o status do personagem (nome, classe, nível, HP e XP)
    bool estaVivo();    // bool para verificar se o personagem está vivo (HP > 0)
};

#endif // JOGADOR_HPP