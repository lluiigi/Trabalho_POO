#include <iostream>
#include "armas.hpp"

Arma::Arma(string nome, string descricao, float peso, int dano, int municao,
           string categoria, float chance_aparecer)
    : Item(nome, descricao, peso),
      dano(dano),
      municao(municao),
      categoria(categoria),
      chance_aparecer(chance_aparecer) {}

int Arma::getDano() const {
    return dano;
}

int Arma::getMunicao() const {
    return municao;
}

string Arma::getCategoria() const {
    return categoria;
}

float Arma::getChanceAparecer() const {
    return chance_aparecer;
}

void Arma::usar() {
    if (categoria == "Fogo") {
        if (municao > 0) {
            municao--;
            cout << "*BANG* Você disparou! Munição restante: " << municao << endl;
        } else {
            cout << "*Click* Arma vazia! Sem munição." << endl;
        }
    } else {
        cout << "Você balançou a arma com força." << endl;
    }
}

vector<Arma*> criarListaDeArmas() {
    vector<Arma*> armas;

    // ===== ARMAS BRANCAS (Munição = 0) =====
    armas.push_back(new Arma("Cano Enferrujado", "Pesado e improvisado, fácil de achar em qualquer entulho", 1.5f, 12, 0, "Branca", 0.45f));
    armas.push_back(new Arma("Faca de Cozinha", "Lâmina fina, rápida mas pouco letal contra ossos", 0.3f, 15, 0, "Branca", 0.40f));
    armas.push_back(new Arma("Bastão de Baseball", "Bom alcance e impacto, mas se desgasta com o uso", 1.2f, 20, 0, "Branca", 0.30f));
    armas.push_back(new Arma("Machado de Bombeiro", "Corte profundo, ideal para decapitações, porém lento", 3.0f, 30, 0, "Branca", 0.18f));
    armas.push_back(new Arma("Katana Enferrujada", "Relíquia afiada encontrada raramente, corta com precisão mortal", 1.8f, 38, 0, "Branca", 0.08f));

    // ===== ARMAS DE FOGO (Munição inicial realista) =====
    armas.push_back(new Arma("Pistola .380", "Compacta e comum, encontrada em quase todo lugar", 0.9f, 18, 12, "Fogo", 0.35f));
    armas.push_back(new Arma("Revólver .38", "Confiável, com bom dano por tiro mas pouca capacidade", 1.0f, 25, 6, "Fogo", 0.25f));
    armas.push_back(new Arma("Rifle de Assalto AK", "Cadência alta, ótimo contra hordas, porém pesado e raro", 3.6f, 35, 30, "Fogo", 0.15f));
    armas.push_back(new Arma("Escopeta Pump", "Devastadora a curta distância, recarga lenta", 3.2f, 45, 8, "Fogo", 0.12f));
    armas.push_back(new Arma("Rifle de Precisão .308", "Arma de elite para tiros certeiros à distância, extremamente rara", 4.5f, 60, 5, "Fogo", 0.05f));

    return armas;
}