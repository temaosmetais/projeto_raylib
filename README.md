# Hollow Knight Clone (C + Raylib)

**Disciplina:** Algoritmos e Programação (INF01202) - 2025/02
**Autores:** Lucas Fontes Francisco e Kamille Barcellos Kalinovski

---

## Sobre o Projeto
Releitura do jogo *Hollow Knight* desenvolvida como trabalho final.

### Funcionalidades
* **Modularização:** Código separado em lógica de Mapa, Player, Monstros e Telas.
* **Mapas Dinâmicos:** Fases carregadas a partir de matrizes de caracteres em arquivos de texto.
* **Física:** Colisão AABB precisa para plataformas e inimigos.
* **Gameplay:** Combate corpo a corpo, sistema de vidas e invencibilidade temporária.
* **Câmera:** Sistema 2D com zoom e "clamp" (trava nas bordas do mapa).

---

## Como Rodar

**Importante:** A pasta `assets/` deve estar sempre no mesmo local do executável.

1. Compile o projeto (ou use o executável fornecido). 
    - O projeto pode ser compilado com o seguinte comando no terminal: `gcc src/main.c src/utils.c src/screens.c src/map.c src/player.c src/monster.c -o app.exe -I include/ -L lib/windows/ -lraylib -lopengl32 -lgdi32 -lwinmm -Wall -std=c99`
2. Execute o arquivo.

### Controles

| Tecla | Ação |
| :--- | :--- |
| **Setas ** | Mover |
| **Z** | Pular |
| **X** | Atacar |
| **Seta para Cima** | Entrar em Portais |
| **ESC** | Pausar / Voltar |

---

## Funcionamento
* **Hub (Vila):** Área segura inicial. Use o portal/portão para acessar as fases.
* **Progresso:** São 3 fases de túneis sequenciais com inimigos.
* **Objetivo:** Chegar ao portal final da última fase.
* **Morte:** Ao perder as 5 vidas, você volta para a Vila.

---

## Estrutura de Arquivos

* `main.c`: Loop principal.
* `game_constants.h`: Configurações globais e structs.
* `screens.c`: Controle de fluxo (Menu, Jogo, Morte, Transições).
* `map.c`: Leitor de `.txt` e renderizador do cenário.
* `player.c`: Física, input e colisão do jogador.
* `monster.c`: IA simples dos inimigos.
* `utils.c`: Variáveis auxiliares.
