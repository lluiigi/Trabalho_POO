#include "alimento.hpp"
using namespace std;
#include <iostream>
#include <vector>
Alimento::Alimento(string nome,string descricao, float peso, int recuperacao_hp) // Construtor
    : Item(nome, descricao, peso), cura_hp(recuperacao_hp) {} // Inicializa a classe base Item e o atributo cura_hp

int Alimento::getCuraHP() { // Getter para obter a quantidade de HP que o alimento cura
    return cura_hp; // retorna hp que o alimento cura
}


// void para exibir detalhes do alimento, implementa a função virtual pura da classe base Item
void Alimento::exibirDetalhes(){
    cout << "===== Detalhes do Alimento =====" << endl;
    cout << "Nome: " << nome << endl;
    cout << "Descrição: " << descricao << endl;
    cout << "Peso: " << peso << " kg" << endl;
    cout << "Cura HP: " << cura_hp << endl;


}
vector<Alimento*> criarListaDeAlimentos() {
    vector<Alimento*> alimentos;

    alimentos.push_back(new Alimento("Maca Mordida", "Um pouco oxidada, mas ainda tem algumas vitaminas.", 0.2f, 5));
    alimentos.push_back(new Alimento("Fatia de Pizza Fria", "Achada em uma caixa amassada. Recupera as energias e a moral.", 0.3f, 15));
    alimentos.push_back(new Alimento("Hamburguer de Franquia", "Cheio de conservantes, incrivelmente intacto apos o apocalipse.", 0.4f, 25));
    alimentos.push_back(new Alimento("Temaki de Sobrevivencia", "Um pouco arriscado no fim do mundo, mas revigora a alma.", 0.5f, 40));
    alimentos.push_back(new Alimento("Racao Militar (MRE)", "Refeicao completa em po. Gosto horrivel, mas cura quase tudo.", 1.2f, 80));

    return alimentos;
}
