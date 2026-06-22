import pygame

# 1. Inicializa o motor gráfico
pygame.init()

# 2. Configura o tamanho da janela (Largura x Altura)
largura = 800
altura = 600
tela = pygame.display.set_mode((largura, altura))

# 3. Dá um nome para a janela do seu jogo
pygame.display.set_caption("RPG Zumbi - POO")

# 4. O "Loop Principal" do jogo (Mantém a janela aberta)
rodando = True
while rodando:
    # Checa se o jogador clicou no 'X' para fechar a janela
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            rodando = False
            
    # Pinta o fundo de uma cor (RGB: Vermelho, Verde, Azul)
    # 0, 0, 0 = Preto
    tela.fill((0, 0, 0))
    
    # Atualiza a tela para mostrar os desenhos
    pygame.display.flip()

# 5. Encerra o Pygame corretamente quando sair do loop
pygame.quit()