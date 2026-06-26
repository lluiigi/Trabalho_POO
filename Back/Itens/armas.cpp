#include <iostream>
#include "armas.hpp"

Arma::Arma(string nome, string descricao, float peso, int dano, int municao,string categoria, float chance_aparecer) : Item(nome, descricao, peso), // Inicializa a classe base Item (contrutor)
      dano(dano), //o dano da arma
      municao(municao), //municao da arma
      categoria(categoria), // categoria da arma inicializa a categoria da arma
      chance_aparecer(chance_aparecer) {} // Inicializa os atributos específicos da classe Arma

Arma::~Arma() {} // destrutor da classe Arma


// getters da classe Arma

// acessa o dano da arma e retorna ele 
int Arma::getDano() const {
    return dano;
}
// acessa a munição da arma e retorna ela
int Arma::getMunicao() const {
    return municao;
}
// acessa a categoria da arma e retorna ela
string Arma::getCategoria() const {
    return categoria;
}
// acessa a chance de aparecer da arma e retorna ela
float Arma::getChanceAparecer() const {
    return chance_aparecer;
}
// void feito para usar a arma, se for de fogo, diminui a munição, se não for, apenas balança a arma
void Arma::usar() {
    if (categoria == "Fogo") {
        if (municao > 0) {
            municao--;
            cout << "*BANG* Você disparou! Munição restante: " << municao << endl; // caso seja de fogo, diminui a munição e mostra a quantidade restante
        } else {
            cout << "*Click* Arma vazia! Sem munição." << endl;
        }
    } else {
        cout << "Você balançou a arma com força." << endl; // se for arma branca, apenas balança a arma, sem diminuir munição
    }
}

// void para recarregar a arma com municao
void Arma::recarregar(int qtd) {
    municao += qtd;
}

//void pora exibir detalhes da arma, implementa a função virtual pura da classe base Item
void Arma::exibirDetalhes()  {
    cout << "===== Detalhes da Arma =====" << endl;
    cout << "Nome: " << nome << endl;
    cout << "Descrição: " << descricao << endl;
    cout << "Peso: " << peso << " kg" << endl;
    cout << "Dano: " << dano << endl;
    cout << "Munição: " << municao << endl;
    cout << "Categoria: " << categoria << endl;
    cout << "Chance de Aparecer: " << chance_aparecer * 100 << "%" << endl;
}

// vetor de armas, com todas as armas do jogo, tanto branca quanto de fogo
vector<Arma*> criarListaDeArmas() {
    vector<Arma*> armas;

    // Armas brancas
    armas.push_back(new Arma("Faca", "Lâmina rápida para curtas distâncias.", 0.3f, 15, 0, "Branca", 0.40f));
    armas.push_back(new Arma("Taco de Beisebol", "Bom alcance e impacto.", 1.2f, 20, 0, "Branca", 0.30f));

    // Armas de fogo
    armas.push_back(new Arma("Pistola", "Arma de fogo básica e confiável.", 0.9f, 25, 12, "Fogo", 0.35f));
    armas.push_back(new Arma("Escopeta", "Devastadora de perto.", 3.2f, 45, 8, "Fogo", 0.12f));

    return armas;
}