#include <iostream>
#include "mochila.hpp"

Mochila::Mochila(int capacidade_maxima) : capacidade_maxima(capacidade_maxima) {}

bool Mochila::guardarItem(Item* novoItem) {
    if (itens.size() < static_cast<size_t>(capacidade_maxima)) {
        itens.push_back(novoItem);
        std::cout << "Item guardado na mochila com sucesso!" << std::endl;
        return true;
    } else {
        std::cout << "Mochila cheia! Não foi possível guardar o item." << std::endl;
        return false;
    }
}

void Mochila::exibirItens() {
    if (itens.empty()) {
        std::cout << "A mochila está vazia." << std::endl;
        return;
    }

    std::cout << "===== Itens na Mochila (" << itens.size() << "/" << capacidade_maxima << ") =====" << std::endl;
    for (size_t i = 0; i < itens.size(); i++) {
        std::cout << i + 1 << ". " << itens[i]->getNome() << std::endl;
    }
}