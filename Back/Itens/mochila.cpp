#include <iostream>
#include "mochila.hpp"

Mochila::Mochila(int capacidade_maxima) : capacidade_maxima(capacidade_maxima) {} // Construtor que inicializa a capacidade máxima da mochila


// bool para guardar um item na mochila, verifica se a mochila não está cheia, se não estiver, guarda o item e retorna true, se estiver cheia, retorna false
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

//void para exibir os itens da mochila, mostra o nome de cada item guardado na mochila, e a quantidade de itens guardados
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

Item* Mochila::obterItem(std::string nomeItem) {
    for (size_t i = 0; i < itens.size(); i++) {
        if (itens[i]->getNome() == nomeItem) {
            return itens[i];
        }
    }
    return nullptr; // Retorna nulo se não achar
}

// Remove o item da mochila e do vetor
bool Mochila::removerItem(std::string nomeItem) {
    for (auto it = itens.begin(); it != itens.end(); ++it) {
        if ((*it)->getNome() == nomeItem) {
            itens.erase(it);
            return true;
        }
    }
    return false;
}