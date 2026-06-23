#ifndef ZUMBI_HPP
#define ZUMBI_HPP

#include <string>

// --- CLASSE BASE ---
class Zumbi {
    protected: 
        std::string tipo; 
        int vida;
        int dano;
        int xp_recompensa; // <-- O XP está aqui

    public:
        // 1. RESOLVIDO: Agora o construtor pede as 4 coisas!
        Zumbi(std::string tipo, int vida, int dano, int xp_recompensa); 

        void receberDano(int danoRecebido); 
        virtual void atacar(); 
        bool estaVivo();

        // Getters
        std::string getTipo() const;
        int getVida() const;
        int getDano() const;
        int getXpRecompensa() const; // <-- O Getter do XP está aqui
};

// --- CLASSES FILHAS ---
class ZumbiNormal : public Zumbi {
public:
    ZumbiNormal();
};

class ZumbiTanque : public Zumbi {
public:
    ZumbiTanque();
};

class ZumbiRapido : public Zumbi {
public:
    ZumbiRapido();
};

#endif