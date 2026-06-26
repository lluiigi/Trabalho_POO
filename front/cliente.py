"""
=====================================================================
 CLIENTE.PY - FRONT-END (TELA + CONTROLE) DO RPG DE SOBREVIVENCIA ZUMBI
=====================================================================
"""

import pygame
import socket
import threading
import queue
import random
import sys

# =====================================================================
# CONFIGURAÇÕES GERAIS
# =====================================================================
LARGURA, ALTURA = 800, 600
FPS = 60

HOST_SERVIDOR = "localhost"
PORTA_SERVIDOR = 5000

# Coloque False quando o servidor C++ estiver pronto e rodando.
MODO_SIMULADO = False

TILE = 40
GRID_LARGURA = LARGURA // TILE
GRID_ALTURA = (ALTURA - 100) // TILE  

# Cores (RGB)
PRETO = (0, 0, 0)
BRANCO = (255, 255, 255)
CINZA = (60, 60, 60)
CINZA_CLARO = (160, 160, 160)
VERDE = (40, 180, 70)
VERDE_GRAMA = (34, 139, 34)
AZUL_JOGADOR = (40, 90, 220)
MARROM_CASA = (120, 72, 30)
VERMELHO = (200, 30, 30)
VERMELHO_ESCURO = (120, 10, 10)
AMARELO = (230, 200, 30)
ROXO_ZUMBI = (90, 140, 60)
AZUL_HOVER = (70, 120, 250)


# =====================================================================
# CLASSE: NetworkClient
# =====================================================================
class NetworkClient:
    def __init__(self, host=HOST_SERVIDOR, porta=PORTA_SERVIDOR):
        self.host = host
        self.porta = porta
        self.sock = None
        self.conectado = False
        self.fila_recebidos = queue.Queue()
        self._thread_escuta = None
        self._rodando = False

    def conectar(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(3)  
            self.sock.connect((self.host, self.porta))
            self.sock.settimeout(None) 
            self.conectado = True
            self._rodando = True

            self._thread_escuta = threading.Thread(
                target=self._listen_loop, daemon=True
            )
            self._thread_escuta.start()
            print("[NETWORK] Conectado ao servidor C++ em "
                  f"{self.host}:{self.porta}")
            return True
        except (socket.error, OSError) as e:
            print(f"[NETWORK] Falha ao conectar ao servidor: {e}")
            self.conectado = False
            return False

    def _listen_loop(self):
        buffer = ""
        while self._rodando:
            try:
                dados = self.sock.recv(1024) 
                if not dados:
                    print("[NETWORK] Servidor encerrou a conexão.")
                    self.conectado = False
                    break

                buffer += dados.decode("utf-8", errors="ignore")

                while "\n" in buffer:
                    linha, buffer = buffer.split("\n", 1)
                    linha = linha.strip()
                    if linha:
                        self.fila_recebidos.put(linha)

            except (socket.error, OSError) as e:
                print(f"[NETWORK] Erro na escuta do socket: {e}")
                self.conectado = False
                break

    def enviar(self, mensagem: str):
        if not self.conectado or self.sock is None:
            print(f"[NETWORK] (sem conexão) comando não enviado: {mensagem}")
            return
        try:
            self.sock.sendall((mensagem + "\n").encode("utf-8")) 
        except (socket.error, OSError) as e:
            print(f"[NETWORK] Erro ao enviar dados: {e}")
            self.conectado = False

    def obter_mensagens_pendentes(self):
        mensagens = []
        while not self.fila_recebidos.empty():
            mensagens.append(self.fila_recebidos.get())
        return mensagens

    def fechar(self):
        self._rodando = False
        if self.sock:
            try:
                self.sock.close()
            except OSError:
                pass


# =====================================================================
# ESTADOS DO JOGO
# =====================================================================
ESTADO_SELECAO_CLASSE = "SELECAO_CLASSE"
ESTADO_EXPLORACAO = "EXPLORACAO"
ESTADO_BATALHA = "BATALHA"


# =====================================================================
# CLASSE PRINCIPAL DO JOGO
# =====================================================================
class Jogo:
    def __init__(self):
        pygame.init()
        pygame.display.set_caption("RPG Zumbi - Cliente Pygame")
        self.tela = pygame.display.set_mode((LARGURA, ALTURA))
        self.clock = pygame.time.Clock()

        self.fonte_titulo = pygame.font.SysFont("arial", 42, bold=True)
        self.fonte_normal = pygame.font.SysFont("arial", 22)
        self.fonte_pequena = pygame.font.SysFont("arial", 16)

        self.rodando = True
        self.estado = ESTADO_SELECAO_CLASSE

        self.network = NetworkClient()
        if not MODO_SIMULADO:
            self.network.conectar()

        self.classe_escolhida = None

        self.jogador_grid_x = GRID_LARGURA // 2
        self.jogador_grid_y = GRID_ALTURA // 2
        self.tempo_ultimo_movimento = 0
        self.intervalo_movimento_ms = 160

        self.casas = [
            (2, 2), (2, 3), (10, 4), (15, 8), (5, 9)
        ]

        self.chance_encontro = 0.12 

        self.piscando = False
        self.contador_pisca = 0
        self.qtd_piscadas = 6  

        self.hp_jogador_max = 100
        self.hp_jogador = 100
        self.hp_zumbi_max = 60
        self.hp_zumbi = 60
        self.texto_batalha = "Um Zumbi selvagem apareceu!"
        self.aguardando_resposta_servidor = False
        self.mostrar_mochila = False

        # [NOVO] Mochila começa vazia e texto de exploração dinâmico
        self.itens_mochila = []
        self.texto_exploracao = "Move-te pelo mapa para explorares!"

        self.rect_btn_atacar = pygame.Rect(40, 470, 220, 50)
        self.rect_btn_mochila = pygame.Rect(290, 470, 220, 50)
        self.rect_btn_fugir = pygame.Rect(540, 470, 220, 50)
        self.rect_btn_lutador = pygame.Rect(LARGURA // 2 - 260, 320, 220, 80)
        self.rect_btn_atirador = pygame.Rect(LARGURA // 2 + 40, 320, 220, 80)

    def rodar(self):
        while self.rodando:
            dt = self.clock.tick(FPS)
            self.processar_eventos()
            self.processar_rede()  

            if self.estado == ESTADO_SELECAO_CLASSE:
                self.desenhar_selecao_classe()
            elif self.estado == ESTADO_EXPLORACAO:
                self.atualizar_exploracao(dt)
                self.desenhar_exploracao()
            elif self.estado == ESTADO_BATALHA:
                self.desenhar_batalha()

            pygame.display.flip()

        self.network.fechar()
        pygame.quit()
        sys.exit()

    def processar_eventos(self):
        for evento in pygame.event.get():
            if evento.type == pygame.QUIT:
                self.rodando = False

            if self.estado == ESTADO_SELECAO_CLASSE:
                self.eventos_selecao_classe(evento)
            elif self.estado == ESTADO_EXPLORACAO:
                self.eventos_exploracao(evento)
            elif self.estado == ESTADO_BATALHA:
                self.eventos_batalha(evento)

    def processar_rede(self):
        if MODO_SIMULADO:
            return  

        mensagens = self.network.obter_mensagens_pendentes()
        for msg in mensagens:
            self.interpretar_mensagem_servidor(msg)

    def interpretar_mensagem_servidor(self, msg: str):
        print(f"[SERVIDOR] {msg}")
        try:
            if msg.startswith("HP_JOGADOR:"):
                self.hp_jogador = int(msg.split(":")[1])
                
            elif msg.startswith("HP_ZUMBI:"):
                self.hp_zumbi = int(msg.split(":")[1])
                
            elif msg.startswith("REMOVER_ITEM:"):
                item_remover = msg.split(":", 1)[1]
                if item_remover in self.itens_mochila:
                    self.itens_mochila.remove(item_remover)
                    
            elif msg.startswith("TEXTO:"):
                self.texto_batalha = msg.split(":", 1)[1]
                self.aguardando_resposta_servidor = False
            
            elif msg.startswith("BATALHA_FIM:"):
                resultado = msg.split(":")[1]
                self.texto_batalha = f"Fim de batalha: {resultado}"
                self.aguardando_resposta_servidor = False
                self.estado = ESTADO_EXPLORACAO

            # [NOVO] Adicionado leitura das mensagens de exploração
            elif msg.startswith("EXPLORAR:NADA"):
                self.texto_exploracao = "Não encontraste nada nesta zona..."

            elif msg.startswith("BAU_CONTEUDO:"):
                item_ganho = msg.split(":", 1)[1]
                self.itens_mochila.append(item_ganho)
                self.texto_exploracao = f"Encontraste um baú! Recebeste: {item_ganho}"

            elif msg.startswith("EXPLORAR:ZUMBI:"):
                nome_zumbi = msg.split(":", 2)[2]
                self.texto_exploracao = f"Atenção! Um {nome_zumbi} aproximou-se!"
                self.iniciar_transicao_para_batalha()
                
        except (ValueError, IndexError) as e:
            print(f"[NETWORK] Mensagem mal formatada '{msg}': {e}")

    def eventos_selecao_classe(self, evento):
        if evento.type == pygame.MOUSEBUTTONDOWN and evento.button == 1:
            pos = evento.pos
            if self.rect_btn_lutador.collidepoint(pos):
                self.escolher_classe("LUTADOR")
            elif self.rect_btn_atirador.collidepoint(pos):
                self.escolher_classe("ATIRADOR")

    def escolher_classe(self, classe: str):
        self.classe_escolhida = classe
        if MODO_SIMULADO:
            print(f"[SIMULADO] Classe escolhida: {classe} ")
        else:
            if not self.network.conectado:
                self.network.conectar()
            self.network.enviar(f"CLASSE:{classe}")

        self.estado = ESTADO_EXPLORACAO

    def desenhar_selecao_classe(self):
        self.tela.fill((20, 20, 30))
        titulo = self.fonte_titulo.render("RPG DE SOBREVIVENCIA ZUMBI", True, BRANCO)
        self.tela.blit(titulo, (LARGURA // 2 - titulo.get_width() // 2, 120))
        subtitulo = self.fonte_normal.render("Escolha sua classe para começar:", True, CINZA_CLARO)
        self.tela.blit(subtitulo, (LARGURA // 2 - subtitulo.get_width() // 2, 220))

        mouse_pos = pygame.mouse.get_pos()

        cor_lutador = AZUL_HOVER if self.rect_btn_lutador.collidepoint(mouse_pos) else VERMELHO_ESCURO
        pygame.draw.rect(self.tela, cor_lutador, self.rect_btn_lutador, border_radius=10)
        pygame.draw.rect(self.tela, BRANCO, self.rect_btn_lutador, width=2, border_radius=10)
        texto_lutador = self.fonte_normal.render("Lutador", True, BRANCO)
        self.tela.blit(texto_lutador, (self.rect_btn_lutador.centerx - texto_lutador.get_width() // 2, self.rect_btn_lutador.centery - texto_lutador.get_height() // 2))

        cor_atirador = AZUL_HOVER if self.rect_btn_atirador.collidepoint(mouse_pos) else CINZA
        pygame.draw.rect(self.tela, cor_atirador, self.rect_btn_atirador, border_radius=10)
        pygame.draw.rect(self.tela, BRANCO, self.rect_btn_atirador, width=2, border_radius=10)
        texto_atirador = self.fonte_normal.render("Atirador", True, BRANCO)
        self.tela.blit(texto_atirador, (self.rect_btn_atirador.centerx - texto_atirador.get_width() // 2, self.rect_btn_atirador.centery - texto_atirador.get_height() // 2))

        status = "MODO SIMULADO (sem servidor)" if MODO_SIMULADO else ("Conectado ao servidor" if self.network.conectado else "Desconectado do servidor")
        cor_status = AMARELO if MODO_SIMULADO else (VERDE if self.network.conectado else VERMELHO)
        texto_status = self.fonte_pequena.render(status, True, cor_status)
        self.tela.blit(texto_status, (20, ALTURA - 30))

    def eventos_exploracao(self, evento):
        pass  

    def atualizar_exploracao(self, dt):
        if self.piscando:
            self.atualizar_efeito_pisca(dt)
            return  

        agora = pygame.time.get_ticks()
        if agora - self.tempo_ultimo_movimento < self.intervalo_movimento_ms:
            return

        teclas = pygame.key.get_pressed()
        dx, dy = 0, 0

        if teclas[pygame.K_w] or teclas[pygame.K_UP]: dy = -1
        elif teclas[pygame.K_s] or teclas[pygame.K_DOWN]: dy = 1
        elif teclas[pygame.K_a] or teclas[pygame.K_LEFT]: dx = -1
        elif teclas[pygame.K_d] or teclas[pygame.K_RIGHT]: dx = 1

        if dx != 0 or dy != 0:
            novo_x = max(0, min(GRID_LARGURA - 1, self.jogador_grid_x + dx))
            novo_y = max(0, min(GRID_ALTURA - 1, self.jogador_grid_y + dy))

            if (novo_x, novo_y) not in self.casas:
                self.jogador_grid_x = novo_x
                self.jogador_grid_y = novo_y
                self.tempo_ultimo_movimento = agora

                # [NOVO] Comunica com o C++ que andaste, em vez de fazer RNG local
                if not MODO_SIMULADO:
                    self.network.enviar("ANDAR")
                else:
                    self.checar_encontro_aleatorio()

    def checar_encontro_aleatorio(self):
        if random.random() < self.chance_encontro:
            self.iniciar_transicao_para_batalha()

    def iniciar_transicao_para_batalha(self):
        self.piscando = True
        self.contador_pisca = 0
        self._timer_pisca = pygame.time.get_ticks()

    def atualizar_efeito_pisca(self, dt):
        agora = pygame.time.get_ticks()
        if agora - self._timer_pisca > 100:  
            self._timer_pisca = agora
            self.contador_pisca += 1
            if self.contador_pisca >= self.qtd_piscadas:
                self.piscando = False
                self.entrar_em_batalha()

    def entrar_em_batalha(self):
        self.hp_jogador = self.hp_jogador_max
        self.hp_zumbi = self.hp_zumbi_max
        self.texto_batalha = "Um Zumbi selvagem apareceu!"
        self.mostrar_mochila = False
        self.estado = ESTADO_BATALHA

        if MODO_SIMULADO:
            pass
        else:
            self.network.enviar("ENCONTRO:INICIAR")

    def desenhar_exploracao(self):
        self.tela.fill(VERDE_GRAMA)

        for gx in range(GRID_LARGURA + 1):
            pygame.draw.line(self.tela, (28, 110, 28), (gx * TILE, 0), (gx * TILE, GRID_ALTURA * TILE))
        for gy in range(GRID_ALTURA + 1):
            pygame.draw.line(self.tela, (28, 110, 28), (0, gy * TILE), (GRID_LARGURA * TILE, gy * TILE))

        for (cx, cy) in self.casas:
            rect_casa = pygame.Rect(cx * TILE, cy * TILE, TILE, TILE)
            pygame.draw.rect(self.tela, MARROM_CASA, rect_casa)
            pygame.draw.rect(self.tela, (60, 35, 10), rect_casa, width=2)

        rect_jogador = pygame.Rect(self.jogador_grid_x * TILE, self.jogador_grid_y * TILE, TILE, TILE)
        pygame.draw.rect(self.tela, AZUL_JOGADOR, rect_jogador)
        pygame.draw.rect(self.tela, BRANCO, rect_jogador, width=2)

        # [NOVO] HUD atualizado para exibir textos dinâmicos
        pygame.draw.rect(self.tela, PRETO, (0, GRID_ALTURA * TILE, LARGURA, ALTURA - GRID_ALTURA * TILE))
        info = (
            f"Classe: {self.classe_escolhida or '???'}    "
            f"WASD/Setas para mover    "
            f"Mochila: {len(self.itens_mochila)} itens"
        )
        texto_info = self.fonte_pequena.render(info, True, BRANCO)
        self.tela.blit(texto_info, (10, GRID_ALTURA * TILE + 10))

        texto_evento = self.fonte_normal.render(self.texto_exploracao, True, AMARELO)
        self.tela.blit(texto_evento, (10, GRID_ALTURA * TILE + 35))

        if self.piscando and self.contador_pisca % 2 == 0:
            overlay = pygame.Surface((LARGURA, ALTURA))
            overlay.fill(BRANCO)
            overlay.set_alpha(180)
            self.tela.blit(overlay, (0, 0))

    def eventos_batalha(self, evento):
        if evento.type == pygame.KEYDOWN:
            if self.mostrar_mochila:
                if evento.key == pygame.K_ESCAPE:
                    self.mostrar_mochila = False
                return

            if evento.key == pygame.K_1: self.acao_atacar()
            elif evento.key == pygame.K_2: self.mostrar_mochila = True
            elif evento.key == pygame.K_3: self.acao_fugir()

        if evento.type == pygame.MOUSEBUTTONDOWN and evento.button == 1:
            pos = evento.pos
            if self.mostrar_mochila:
                self.mostrar_mochila = False
                return

            if self.rect_btn_atacar.collidepoint(pos): self.acao_atacar()
            elif self.rect_btn_mochila.collidepoint(pos): self.mostrar_mochila = True
            elif self.rect_btn_fugir.collidepoint(pos): self.acao_fugir()

    def acao_atacar(self):
        if self.aguardando_resposta_servidor: return
        if MODO_SIMULADO:
            self.simular_resposta_servidor("ATACAR")
        else:
            self.aguardando_resposta_servidor = True
            self.texto_batalha = "Aguardando o servidor..."
            self.network.enviar("ATACAR")

    def acao_usar_item(self, item: str):
        self.mostrar_mochila = False
        if self.aguardando_resposta_servidor: return
        if MODO_SIMULADO:
            self.simular_resposta_servidor(f"MOCHILA:{item}")
        else:
            self.aguardando_resposta_servidor = True
            self.texto_batalha = f"A usar {item}..."
            self.network.enviar(f"MOCHILA:{item}")

    def acao_fugir(self):
        if self.aguardando_resposta_servidor: return
        if MODO_SIMULADO:
            self.simular_resposta_servidor("FUGIR")
        else:
            self.aguardando_resposta_servidor = True
            self.texto_batalha = "Tentando fugir..."
            self.network.enviar("FUGIR")

    def simular_resposta_servidor(self, comando: str):
        if comando == "ATACAR":
            dano_causado = random.randint(10, 25)
            self.hp_zumbi = max(0, self.hp_zumbi - dano_causado)
            self.texto_batalha = f"Atacaste e causaste {dano_causado} de dano!"
            if self.hp_zumbi <= 0:
                self.texto_batalha = "O Zumbi foi derrotado! Venceste!"
                self.estado = ESTADO_EXPLORACAO
                return
            dano_recebido = random.randint(5, 18)
            self.hp_jogador = max(0, self.hp_jogador - dano_recebido)
            if self.hp_jogador <= 0:
                self.texto_batalha = "Foste derrotado pelo zumbi..."
                self.estado = ESTADO_EXPLORACAO
        elif comando.startswith("MOCHILA:"):
            item = comando.split(":", 1)[1]
            cura = random.randint(10, 20)
            self.hp_jogador = min(self.hp_jogador_max, self.hp_jogador + cura)
            self.texto_batalha = f"Usaste {item} e recuperaste {cura} de HP!"
        elif comando == "FUGIR":
            if random.random() < 0.6:
                self.texto_batalha = "Conseguiste fugir!"
                self.estado = ESTADO_EXPLORACAO
            else:
                self.texto_batalha = "Não foi possível fugir!"
                dano_recebido = random.randint(5, 15)
                self.hp_jogador = max(0, self.hp_jogador - dano_recebido)

    def desenhar_batalha(self):
        self.tela.fill((40, 40, 60))  

        zumbi_rect = pygame.Rect(LARGURA - 220, 60, 100, 100)
        pygame.draw.rect(self.tela, ROXO_ZUMBI, zumbi_rect)
        pygame.draw.rect(self.tela, PRETO, zumbi_rect, width=3)
        self.desenhar_barra_hp(LARGURA - 260, 30, 200, "Zumbi", self.hp_zumbi, self.hp_zumbi_max)

        jogador_rect = pygame.Rect(120, 260, 90, 90)
        pygame.draw.rect(self.tela, AZUL_JOGADOR, jogador_rect)
        pygame.draw.rect(self.tela, PRETO, jogador_rect, width=3)
        self.desenhar_barra_hp(40, 380, 200, self.classe_escolhida or "Jogador", self.hp_jogador, self.hp_jogador_max)

        caixa_texto = pygame.Rect(20, 430, LARGURA - 40, 30)
        pygame.draw.rect(self.tela, BRANCO, caixa_texto, border_radius=6)
        pygame.draw.rect(self.tela, PRETO, caixa_texto, width=2, border_radius=6)
        texto_render = self.fonte_pequena.render(self.texto_batalha, True, PRETO)
        self.tela.blit(texto_render, (caixa_texto.x + 10, caixa_texto.y + 6))

        mouse_pos = pygame.mouse.get_pos()
        self.desenhar_botao_acao(self.rect_btn_atacar, "[1] ATACAR", VERMELHO, mouse_pos)
        self.desenhar_botao_acao(self.rect_btn_mochila, "[2] MOCHILA", (150, 110, 30), mouse_pos)
        self.desenhar_botao_acao(self.rect_btn_fugir, "[3] FUGIR", CINZA, mouse_pos)

        if self.aguardando_resposta_servidor:
            aviso = self.fonte_pequena.render("Aguardando resposta do servidor...", True, AMARELO)
            self.tela.blit(aviso, (20, 560))

        if self.mostrar_mochila:
            self.desenhar_mochila()

    def desenhar_barra_hp(self, x, y, largura, nome, hp_atual, hp_max):
        altura_barra = 16
        proporcao = max(0, hp_atual) / hp_max if hp_max > 0 else 0
        nome_render = self.fonte_pequena.render(f"{nome}  HP: {hp_atual}/{hp_max}", True, BRANCO)
        self.tela.blit(nome_render, (x, y - 18))
        pygame.draw.rect(self.tela, VERMELHO_ESCURO, (x, y, largura, altura_barra))
        pygame.draw.rect(self.tela, VERDE, (x, y, int(largura * proporcao), altura_barra))
        pygame.draw.rect(self.tela, BRANCO, (x, y, largura, altura_barra), width=2)

    def desenhar_botao_acao(self, rect, texto, cor_base, mouse_pos):
        cor = AZUL_HOVER if rect.collidepoint(mouse_pos) else cor_base
        pygame.draw.rect(self.tela, cor, rect, border_radius=8)
        pygame.draw.rect(self.tela, BRANCO, rect, width=2, border_radius=8)
        texto_render = self.fonte_normal.render(texto, True, BRANCO)
        self.tela.blit(texto_render, (rect.centerx - texto_render.get_width() // 2, rect.centery - texto_render.get_height() // 2))

    def desenhar_mochila(self):
        largura_janela, altura_janela = 360, 220
        x = LARGURA // 2 - largura_janela // 2
        y = ALTURA // 2 - altura_janela // 2
        janela = pygame.Rect(x, y, largura_janela, altura_janela)

        pygame.draw.rect(self.tela, (230, 230, 230), janela, border_radius=10)
        pygame.draw.rect(self.tela, PRETO, janela, width=3, border_radius=10)

        titulo = self.fonte_normal.render("MOCHILA", True, PRETO)
        self.tela.blit(titulo, (janela.centerx - titulo.get_width() // 2, y + 12))

        mouse_pos = pygame.mouse.get_pos()
        for i, item in enumerate(self.itens_mochila):
            item_rect = pygame.Rect(x + 20, y + 55 + i * 45, largura_janela - 40, 36)
            cor = AZUL_HOVER if item_rect.collidepoint(mouse_pos) else CINZA_CLARO
            pygame.draw.rect(self.tela, cor, item_rect, border_radius=6)
            pygame.draw.rect(self.tela, PRETO, item_rect, width=1, border_radius=6)
            texto_item = self.fonte_pequena.render(item, True, PRETO)
            self.tela.blit(texto_item, (item_rect.x + 10, item_rect.centery - 8))

            if pygame.mouse.get_pressed()[0] and item_rect.collidepoint(mouse_pos):
                self.acao_usar_item(item)

        dica = self.fonte_pequena.render("ESC ou clique fora para fechar", True, (90, 90, 90))
        self.tela.blit(dica, (janela.centerx - dica.get_width() // 2, y + altura_janela - 26))

if __name__ == "__main__":
    print("Iniciando o jogo...") 
    jogo = Jogo()
    print("Objeto Jogo criado.") 
    jogo.rodar()
    print("Fim do programa.")