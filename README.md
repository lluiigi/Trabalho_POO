# Projeto RPG de Zumbi - POO

Este projeto é um jogo de RPG de sobrevivência zumbi baseado em terminal (CLI). Ele foi desenvolvido como um trabalho prático para a disciplina de Programação Orientada a Objetos (POO)[cite: 2], com o intuito de aplicar conceitos fundamentais de engenharia de software no desenvolvimento de jogos.

## 💻 Linguagens e Tecnologias

* **Linguagem:** C++ (Padrão C++17)[cite: 2]
* **Bibliotecas Padrão:** `<iostream>`, `<string>`
* **Paradigmas Utilizados:** Programação Orientada a Objetos (Encapsulamento, Herança, Polimorfismo e Sobrecarga)[cite: 2].

## 🏗️ Arquitetura do Projeto

O sistema foi estruturado de forma modular e escalável, separando as entidades do jogo em arquivos distintos. A arquitetura é dividida da seguinte maneira:

* **Declarações (`.hpp`):** Arquivos de cabeçalho onde as classes (como `Jogador` e `Atirador`) têm seus atributos e métodos mapeados de forma limpa.
* **Implementações (`.cpp`):** Arquivos onde a lógica de funcionamento, regras de negócio e matemática do jogo (como receber dano e subir de nível) são programadas.
* **Motor do Jogo (`main.cpp`):** O arquivo principal que amarra todas as classes, gerencia a interação com o teclado do usuário, exibe os menus e roda a simulação principal.

## 🚀 Como Executar

Para rodar este projeto localmente, é necessário ter um compilador C++ (como o GCC/G++) configurado no seu sistema.

**1. Compilar o código:**
Abra o terminal na raiz do projeto e compile todos os arquivos `.cpp` juntos gerando um executável. Exemplo de comando:
```bash
g++ main.cpp jogador.cpp atirador.cpp -o jogo_zumbi