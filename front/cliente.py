import socket

HOST = '127.0.0.1'
PORT = 5000

print("--- FRONT-END PYTHON ---")
nome_digitado = input("Digite o nome do seu sobrevivente: ")

cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cliente.connect((HOST, PORT))

mensagem = f"Criar_Personagem:{nome_digitado}"
cliente.sendall(mensagem.encode('utf-8'))

# --- NOVO: Esperando a resposta do C++ ---
resposta_cpp = cliente.recv(1024).decode('utf-8')
print(f"\n[RESPOSTA DO JOGO]: {resposta_cpp}")

cliente.close()