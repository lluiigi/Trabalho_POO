#include "armas.hpp"
#include <iostream>

using namespace std;

Arma::Arma(string n, string desc, float p, int d, int m, string categoria) : Item(n, desc, p) {
    dano = d;
    municao = m;
    Tipo = categoria;
}

Arma::~Arma() {
    cout << "[Memoria]: A arma '" << nome << "' foi destruida." << endl;
}

int Arma::getDano() {
    return dano;
}

int Arma::getMunicao() {
    return municao;
}

string Arma::getTipo() {
    return Tipo;
}

#include "armas.hpp"
#include <iostream>

using namespace std;

Arma::Arma(string n, string desc, float p, int d, int m, string categoria) : Item(n, desc, p) {
    dano = d;
    municao = m;
    Tipo = categoria;
}

Arma::~Arma() {
    cout << "[Memoria]: A arma '" << nome << "' foi destruida." << endl;
}

int Arma::getDano() {
    return dano;
}

int Arma::getMunicao() {
    return municao;
}

string Arma::getTipo() {
    return Tipo;
}

void Arma::usar() {
    if (municao > 0) {
        municao--;
        cout << nome << " foi usada! Munição restante: " << municao << endl;
    } else {
        cout << nome << " esta sem munição!" << endl;
    }
}