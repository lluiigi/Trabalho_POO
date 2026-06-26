#include "fabricas.hpp"
#include <iostream>

// ==========================================
// APLICAÇÃO DO PADRÃO FACTORY - JOGADOR
// ==========================================
Jogador* JogadorFactory::criar_jogador(std::string classe, std::string nome) {
    Jogador* novoJogador = nullptr;
    
    if (classe == "Atirador" || classe == "ATIRADOR") {
        novoJogador = new Atirador(nome, "Atirador");
        
        // Criamos uma pistola personalizada com 5 balas em vez do padrão de 12
        Arma* pistolaInicial = new Arma("Pistola", "Arma inicial", 0.9f, 25, 5, "Fogo", 0.0f);
        
        novoJogador->pegarItem(pistolaInicial); 
        
    } else {
        // Lutador começa sem nada
        novoJogador = new Jogador(nome, "Lutador");
    }
    
    return novoJogador;
}

// ==========================================
// APLICAÇÃO DO PADRÃO FACTORY - ITENS
// ==========================================
Arma* ItemFactory::criar_arma(std::string tipo) {
    if (tipo == "Pistola") 
        return new Arma("Pistola", "Arma de fogo básica", 0.9f, 25, 12, "Fogo", 0.35f);
    if (tipo == "Escopeta") 
        return new Arma("Escopeta", "Devastadora de perto", 3.2f, 45, 6, "Fogo", 0.12f);
    if (tipo == "Faca") 
        return new Arma("Faca", "Lâmina rápida", 0.3f, 15, 0, "Branca", 0.40f);
    if (tipo == "Taco de Beisebol") 
        return new Arma("Taco de Beisebol", "Bom alcance", 1.2f, 20, 0, "Branca", 0.30f);
    
    return new Arma(tipo, "Arma encontrada", 1.0f, 10, 5, "Branca", 0.1f);
}

Alimento* ItemFactory::criar_alimento(std::string tipo) {
    if (tipo == "Enlatado") 
        return new Alimento("Enlatado", "Comida em conserva", 0.5f, 20);
    if (tipo == "Kit Primeiros Socorros") 
        return new Alimento("Kit Primeiros Socorros", "Cura alta", 1.0f, 50);
    if (tipo == "Antibiotico") 
        return new Alimento("Antibiotico", "Cura média", 0.2f, 35);
    if (tipo == "Barra de Cereal")
        return new Alimento("Barra de Cereal", "Lanche rápido", 0.1f, 15);
    
    return new Alimento(tipo, "Suprimento", 0.5f, 10);
}