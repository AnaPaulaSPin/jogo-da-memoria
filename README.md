# Jogo da Memória em C

**Descrição:**
Este projeto consiste em um jogo da memória desenvolvido em C, onde os jogadores devem encontrar pares de cartas iguais. O jogo foi criado para concluir a matéria de **Linguagem de Programação I** no semestre 2024.2. Ele permite de 1 a 3 jogadores, com níveis de dificuldade variados, vidas e chances de usar um recurso especial chamado *coringa*.

---

## Estruturas do Projeto

### 1. Estruturas de Dados

* **`cartas`**: representa cada carta do tabuleiro, armazenando:

  * `Carta_num`: número da carta.
  * `estado`: indica se a carta já foi revelada permanentemente.
  * `estadotemp`: usada para mostrar a carta temporariamente quando selecionada.
* **`jogador`**: armazena informações de cada jogador:

  * `nome`: nome do jogador.
  * `pontuacao`: pontuação acumulada.
  * `chancesCoringas`: quantidade de vezes que pode usar o recurso coringa.
  * `vidas`: quantidade de vidas restantes.

---

### 2. Lógica do Jogo

1. **Menu principal:** permite jogar, ver regras, acessar ranking, créditos ou sair do jogo.
2. **Escolha de dificuldade:** define tamanho do tabuleiro:

   * Fácil: 4x4
   * Médio: 6x6
   * Difícil: 8x8
     Cada nível define também o número de chances coringa e vidas.
3. **Aleatorização:** as cartas são embaralhadas aleatoriamente para garantir partidas diferentes a cada jogo.
4. **Turnos:** os jogadores se revezam para escolher duas cartas:

   * Se as cartas forem iguais: ponto para o jogador.
   * Se forem diferentes: perdem uma vida.
5. **Coringa:** permite revelar todas as cartas temporariamente por 5 segundos.
6. **Fim de jogo:** ocorre quando todos os pares forem encontrados ou quando os jogadores desistirem. O ranking é atualizado automaticamente.

---

### 3. Funções Principais

* **Gerais e utilitárias:**

  * `mostrarMenu()`: exibe o menu principal.
  * `regras()`: mostra as regras do jogo.
  * `creditos()`: exibe créditos com efeito de digitação.
  * `imprimirComDelay()`: imprime texto caractere por caractere com atraso.

* **Jogo:**

  * `game()`: função principal que controla o fluxo do jogo.
  * `Aleatorizar()`: embaralha as cartas e preenche a matriz.
  * `Dificuldade()`: seleciona o tamanho do tabuleiro.
  * `escolha()`: captura a posição das cartas escolhidas pelo jogador.
  * `Pares()`: valida se o par escolhido está correto.
  * `MostrarMatriz()`: exibe o tabuleiro atualizado.
  * `mostrarCoringa()`: revela todas as cartas por 5 segundos.

* **Jogadores:**

  * `jogadores()`: aloca e inicializa jogadores.
  * `exibirjogador()`: mostra o jogador da vez, pontuação, coringas e vidas.
  * `chanceCoringa()` e `chanceVidas()`: definem recursos de acordo com a dificuldade.
  * `coringa()`: ativa o recurso coringa.
  * `verificarvidas()`: checa se algum jogador ainda pode jogar.

* **Arquivos e ranking:**

  * `CriarArquivo()`, `rank()`, `RegistrarRank()`, `OrdemRank()`: funções para criar, registrar e exibir o ranking dos jogadores.
  * `ArqAtuais()`, `Atuais()`, `EscreverAtuais()`: controle de quantidade de jogadores já cadastrados.

---

### 4. Diferenciais do Jogo

* Controle de **várias cores** para facilitar visualização do tabuleiro.
* Suporte para **1 a 3 jogadores**, com turnos alternados.
* **Sistema de coringas e vidas**, tornando o jogo mais estratégico.
* **Ranking automático** baseado nas pontuações finais.
* **Validações robustas** de entradas, prevenindo erros de usuário.

---

### 5. Exemplo de Funcionamento

* Jogador escolhe duas cartas pelo número da linha e coluna.
* Cartas iguais: pontuação +1, permanecem viradas.
* Cartas diferentes: jogador perde uma vida, cartas são viradas novamente.
* Jogador pode usar coringa (-2 -2) para ver todas as cartas por 5 segundos.
* Desistência: digitar -1 -1 encerra a participação do jogador.
