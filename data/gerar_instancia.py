import random

W = 2500
V = 1900
N = 40
NUM_INSTANCIAS = 10

# Intervalos dos valores (você pode ajustar se quiser)
PESO_MIN, PESO_MAX = 1, 350
VOLUME_MIN, VOLUME_MAX = 1, 350
VALOR_MIN, VALOR_MAX = 1, 1000

for k in range(1, NUM_INSTANCIAS + 1):
    nome_arquivo = f"data/instancia{k}.txt"
    with open(nome_arquivo, "w") as f:
        # primeira linha
        f.write(f"{W} {V}\n")
        
        # itens
        for i in range(N):
            peso = random.randint(PESO_MIN, PESO_MAX)
            volume = random.randint(VOLUME_MIN, VOLUME_MAX)
            valor = random.randint(VALOR_MIN, VALOR_MAX)
            f.write(f"{peso}\t{volume}\t{valor}\n")
    
    print(f"{nome_arquivo} gerado com sucesso!")
