#ifndef ZUMBI_HPP
#define ZUMBI_HPP

#include <string>

// classe base
class Zumbi{

    private:
        std::string tipo; // Zumbi pode ser 'lento', 'corredor' ou 'tanque'
        int vida;
        int dano;

    public:
        Zumbi(std::string tipo, int vida, int dano); //Construtor de Zumbi

        //Principais ações
        void receberDano(int danoRecebido);
        void atacar(); // pode servir futuramente para a classe Jogador
        bool estaVivo();

        //Getters pra acessar recursos pivados
        std::string getTipo() const;
        int getVida() const;
        int getDano() const;
};

#endif