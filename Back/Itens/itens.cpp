#include "itens.hpp"
#include <iostream>

using namespace std;

// Construtor
Item::Item(string n, string desc, float p) {
    nome = n;
    descricao = desc;
    peso = p;
};

// Destrutor
Item::~Item() {
    cout << "[Memoria]: O item '" << nome << "' foi destruido." << endl;
}

// Getters (Retornam os valores encapsulados)
string Item::getNome() {
    return nome;
}

float Item::getPeso() {
    return peso;
}

// Exibe as informações do item na tela
void Item::exibirDetalhes() {
    cout << "Item: " << nome << " | Peso: " << peso << "kg" << endl;
    cout << "Descricao: " << descricao << endl;
}