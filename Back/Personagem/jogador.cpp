#include "jogador.hpp"
#include "zumbi.hpp"
#include <iostream>

using namespace std;


// void para setar o nivel do jogador, se for menor que 0, seta para 0
void Jogador::setNivel(int valor) {
    if (valor < 0) nivel = 0;
    else nivel = valor;
}

//void para setar a vida do jogador, se for maior que a vida máxima, seta para a vida máxima, se for menor que 0, seta para 0
void Jogador::setHP(int valor) {
    if (valor > hp_maximo) hp = hp_maximo;
    else if (valor < 0) hp = 0;
    else hp = valor;
}


//void para setar a experiencia do jogador, se for menor que 0, seta para 0
void Jogador::setXP(int valor) {
    if (valor < 0) xp = 0;
    else xp = valor;
}

// Construtor Completo (ÚNICA versão — duplicata removida)
Jogador::Jogador(string n, string c, int niv, int hpm, int xp_inicial) {
    nome = n;
    classe = c;
    hp_maximo = hpm;
    dano_base = 10;
    armaEquipada = nullptr;
    mochila = new Mochila(5);
    setNivel(niv);
    setHP(hpm);
    setXP(xp_inicial);
}

// Construtor Inicial
Jogador::Jogador(string n, string c) {
    nome = n;
    classe = c;
    hp_maximo = 100;
    nivel = 1;
    hp = 100;
    xp = 0;
    dano_base = 10;
    armaEquipada = nullptr;
    mochila = new Mochila(5);
}

//destutor do jogador, libera a memoria da mochila e mostra uma mensagem de destruição do objeto
Jogador::~Jogador() {
    delete mochila;
    cout << "[Destrutores]: Objeto do jogador '" << nome << "' foi liberado." << endl;
}

// gettter para obter o nome do jogador
bool Jogador::estaVivo() {
    return hp > 0;
}

// getter para obter o nome do jogador
void Jogador::receber_dano(int quantidade) {
    if (quantidade > 0) setHP(hp - quantidade);
}




//void para ganhar experiencia, recebe a quantidade de experiencia que será adicionada a experiencia do jogador
void Jogador::ganhar_xp(int quantidade) {
    if (quantidade > 0) {
        setXP(xp + quantidade);
        cout << nome << " ganhou " << quantidade << " de XP!" << endl;
        if (xp >= 100) subir_nivel();
    }
}

//void para subir de nivel, aumenta o nivel do jogador, aumenta a vida máxima e seta a vida atual para a vida máxima, reseta a experiencia para 0
void Jogador::subir_nivel() {
    nivel++;
    xp = xp - 100;
    hp_maximo += 20;
    hp = hp_maximo;
    cout << "--- LEVEL UP! " << nome << " alcancou o Nivel " << nivel << "! ---" << endl;
}

//void para exibir o status do jogador, exibe nome, classe, nivel, vida atual/vida máxima e experiencia atual
void Jogador::exibirStatus() {
    cout << "Nome: " << nome << " (" << classe << ") | Nivel: " << nivel
         << " | HP: " << hp << "/" << hp_maximo
         << " | XP: " << xp << endl;
}

// combate 
void Jogador::equiparArma(Arma* novaArma) {
    armaEquipada = novaArma;
    if (armaEquipada != nullptr) {
        cout << "[Inventario]: " << nome << " equipou a arma!" << endl;
    }
}

// Função padrão. Lutador e Atirador sobrescrevem ela!
void Jogador::atacar(Zumbi* alvo) {
    if (!estaVivo()) return;

    int danoTotal = dano_base;
    if (armaEquipada != nullptr) {
        danoTotal += armaEquipada->getDano();
        armaEquipada->usar();
    }

    cout << nome << " atacou e causou " << danoTotal << " de dano!" << endl;
    alvo->receberDano(danoTotal);
}
//voi para pegar un item 
void Jogador::pegarItem(Item* novoItem) {
    if (novoItem != nullptr) {
        // Tenta guardar na mochila. Se retornar true, deu certo!
        bool guardou = mochila->guardarItem(novoItem); 
        if (guardou) {
            cout << nome << " guardou " << novoItem->getNome() << " na mochila." << endl;
        }
    }
}

// void para ver o conteúdo da mochila, chama a função exibirItens() da classe Mochila
void Jogador::verMochila() {
    cout << "\n--- Mochila de " << nome << " ---" << endl;
    mochila->exibirItens(); // Chama a função que criamos lá na classe Mochila
    cout << "-----------------------" << endl;
}

//void para comer um alimento, recebe um ponteiro para o alimento que será comido, aumenta a vida do jogador de acordo com a quantidade de cura do alimento, e remove o item da mochila
void Jogador::comer(Alimento* comida) {
    if (comida != nullptr) {
        int cura = comida->getCuraHP();
        curar(cura); // Chama a sua função que já impede a vida de passar do máximo!
        cout << "[Acao]: " << nome << " consumiu " << comida->getNome() 
             << " e recuperou " << cura << " de HP!" << endl;
             
        // Se a comida for de uso único, você pode deletá-la da memória aqui
        // delete comida; 
    }

}
// Getter para obter o nome do jogador
 string Jogador::getNome() {
    return nome;
}

//getter para obter a vida atual do jogador
int Jogador::getVida() const {
    return hp; // retorna a vida do jogador
}
//getter para curar o jogador, recebe a quantidade de vida que será adicionada a vida do jogador
void Jogador::curar(int quantidade) {
    hp += quantidade;
    // Impede que a vida passe do limite máximo (ex: 100)
    if (hp > 100) {
        hp = 100; 
    }
    
}