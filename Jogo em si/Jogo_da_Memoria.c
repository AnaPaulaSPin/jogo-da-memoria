#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <unistd.h> 

//  Variavies que armazenam os códigos ANSI para cores em tons de lilás
#define RESET          "\x1b[0m"          // cor padrao
#define LILAC_LIGHT    "\x1b[38;5;168m"   // Lilás claro
#define LILAC_DARK     "\x1b[38;5;129m"   // Lilás escuro
#define PURPLE_LIGHT   "\x1b[38;5;135m"   // Roxo claro
#define PURPLE_DARK    "\x1b[38;5;55m"    // Roxo escuro
#define RED            "\x1b[38;5;196m"   // Vermelho
#define BLUE_LIGHT     "\x1b[38;5;75m"    // Azul claro
#define GREEN_LIGHT    "\x1b[38;5;82m"    // Verde claro
#define BLUE_DARK      "\x1b[38;5;21m"    // Azul escuro
#define BLUE_CYAN      "\x1b[38;5;87m"    // Azul ciano


// Definicao da estrutura das cartas
typedef struct {
    int Carta_num; //numero da carta
    int estado; // se estara virada(1) ou desvirada(0)
    int estadotemp; // para manipular qual das cartas foi escolhida e assim mudar de cor temporiamente

} cartas;

//Definicao da estrutura de jogadores
typedef struct{
    char nome[50]; //nome dos jogadores
    int pontuacao; //variavel para registrar a pontuação dos jogadores
    int chancesCoringas; //variavel para armazenar as chances coringas que cada jogador tem
    int vidas;

} jogador;

// Declaracoes das Funcoes
 // funcoes ilustrativas:
  void mostrarMenu();
  void regras(); // exibe as regras do jogo e como funciona
  void MostrarMatriz(); // Mostrar o baralho
  void niveis(); // Explicação dos niveis de dificuldades
  void mostrarCoringa(); //Função para as cartas aparecem todas desviradas por 5 segundos
  void imprimirComDelay();
  void creditos(); // função creditos(motivo da criação do jogo e referencias)

 //Funções da logicas do jogo:
  void game(); // jogo de fato
  void Aleatorizar(); // cria os pares e mistura eles
  int Dificuldade(); // jogador escolhe o modo do jogo
  int escolha(); // escolha da posição das cartas e validação das escolhas
  void Pares(); // Validar os pares escolhidos

  //Funcoes para jogadores:
  jogador* jogadores(); // Alocara quantidade de jogadores
  void exibirjogador(); // mostrar o nome do jogador atual, e a sua pontuação
  void coringa(); //Verificar as chances coringas dos jogadores
  void chanceCoringa(); //Para o inicio do jogo, saber o tamanho da dificuldade e assim quantas chances coringas estarao disponiveis para os jogadores
   void chanceVidas();
   int verificarvidas();


  // Funções basicas:
  void pausar(); // Função para pausar o jogo, dando tempo para o usuário ler as informações antes da limpeza
  void liberarMatriz(); //liberar a matriz alocada

 //Arquivos
 FILE *ArqAtuais(); // Criar um arquivo para armazena o numero de jogadores que jogaram o jogo
 void Atuais(); // Função atualiza(Atribuir) o jogAtuais no inicio do jogo
 void EscreverAtuais(); // Toda vez que o usuario fechar o jogo, vai salvar a quantidade atual de jogadores


//Função para Rank:
   void CriarArquivo(); //Criar arquivo pro Rank
 void rank(); //Criar o arquivo pro rank
 void RegistrarRank(); //Registrar os dados dos jogadores da partida em um arquivo
 jogador * OrdemRank();

//variaveis globais
FILE *Arank; // Ponteiro pro arquivo que registrar as pontuações
int jogAtuais = 0; //armazenar quantos jogadores que ja jogaram o jogo.(vamos utilizar isso no rank)


//Menu principal:
int main(){
    int r;
    srand(time(NULL)); // Inicializa a semente aleatória(para usar na função Aleatorio)
    CriarArquivo(); //Criar arquivo para o rank

    //resgatar o numero atuais de jogadores jogados:
    FILE *ArqJog = ArqAtuais(); // Criar o arquivo que vai registrar os jogadores atuais
    Atuais(ArqJog); // armazena a atual quantidade de jogadores registrados



    while(1){
     mostrarMenu();
     printf(LILAC_LIGHT"Escolha uma opcao: "RESET);
        // Verificação para caso o usuario digite uma letra:
        if (scanf("%d", &r) != 1) {
            // Se a entrada não for um número, exibe uma mensagem e limpa o buffer
            printf(RED"Entrada invalida. Por favor, insira um numero inteiro.\n"RESET);
            while(getchar() != '\n'); // Limpa o buffer de entrada
            pausar();
            system("clear || cls"); // Limpa a tela
            continue; // Volta para o início do loop
        }


    // Limpar o buffer de entrada
        getchar();

        switch (r) {
            //opcap jogar:
            case 1: {
                pausar();
                game();
                pausar();
                system("clear || cls"); // Limpa a tela
                break;
            }
            //opcao para mostrar as regras do jogo
            case 2: {
                system("clear || cls"); // Limpa a tela
                regras();
                pausar();
                system("clear || cls"); // Limpa a tela
                break;
            }
            //opcao para mostrar o rank
            case 3: {
                system("clear || cls"); // Limpa a tela
                rank();
                pausar();
                system("clear || cls"); // Limpa a tela
                break;
            }
            //mostrar os creditos(quem fez e qual foi o proposito)
            case 4: {
                system("clear || cls"); // Limpa a tela
                creditos();
                pausar();
                system("clear || cls"); // Limpa a tela
                break;
            }
            //Sair do jogo
            case 5: {
                printf(RED"Saindo do jogo...\n"RESET);
                EscreverAtuais(ArqJog); // Registra Atualizar o numero de jogadores que jogaram
                pausar();
                break;
            }
            //opcoes invalidas
            default: {
                printf(RED"Opção inválida, tente novamente.\n"RESET);
                pausar();
                system("clear || cls"); // Limpa a tela
            }
        }

        //finaliza o loop caso 5 seja igual a 5 e assim finaliza o jogo
        if (r == 5) {
            break;
        }
    }
    return 0;
}

//Função do jogo principal
void game() {
    int lin1, col1, lin2,col2; // variaveis para armazenar as posições das duas cartas
    int virada=0; //armazena quantas cartas estao sendo viradas ao longo do jogo
    int Njogadores=0; //armazena o numero de jogadores
     int atual=0; //armazena quem é o jogador da vez
    int result; // variavel usada para verificar se o usuario quer continuar jogando

    system("clear || cls"); // Limpa a tela
    // Informar a quantidade de jogadores:
    jogador *jogadoresAtuais = jogadores(&Njogadores);


    // Jogador escolhe a dificuldade e jogo vai retornar o valor que sera alocado, 4x4, 6x6 ou 8x8:
    int n = Dificuldade();

    
    //Alocar as "linhas" da matriz:
    cartas **matriz = (cartas**) malloc(n * sizeof(cartas*));

    // Alocar - "Colunas" da matriz:
    for(int i = 0; i<n;i++){
        matriz[i] = (cartas*) malloc(n*sizeof(cartas));
    }

    // função para determinar quantas chances coringa os usuarios vão ter:
     chanceCoringa(n, jogadoresAtuais, Njogadores);
     chanceVidas(n, jogadoresAtuais, Njogadores);

    // definição da matriz(preenche a matriz e aleatoriza as posições)
      Aleatorizar(matriz,n);

    //inicio do jogo de fato
    while(1){
        //Verificar se o usuario apertou em sair na função Dificuldade:
        if(n == -1){
         printf(RED"Voltando pro menu...\n\n"RESET);
         liberarMatriz(matriz,n);
         free(jogadoresAtuais);
         break;

        }

      //mostrar a matriz:
       MostrarMatriz(matriz, n);
       exibirjogador(jogadoresAtuais,&atual); //exibir a informação dos jogadores

       //O jogador da vez vai escolher a linha e coluna:
       result = escolha(&lin1, &col1, &lin2, &col2, n, matriz, jogadoresAtuais, atual);
       getchar(); // limpar buffer de entrada

       //Caso o usuario digitou -1, o programa vai acabar:
       if(result == -1){
         RegistrarRank(jogadoresAtuais,&Njogadores); //Registra a pontuação e o nome dos jogadores.
         jogAtuais += Njogadores; //Atualizar o total de jogadores que jogaram o jogo

         free(jogadoresAtuais);
         liberarMatriz(matriz,n);
         break;
       }

       //Altera a matriz e ja mostra as cartas escolhidas desviradas em vermelho
       system("clear || cls"); // Limpa a tela
       MostrarMatriz(matriz, n);
       exibirjogador(jogadoresAtuais, &atual);

       //Verificar se os pares sao os certos:
       Pares(matriz,&lin1,&col1,&lin2,&col2,&virada, jogadoresAtuais, atual);


       //verificação se o jogo acabou(O total da variavel viradas será igual a quantindade de cartas):
       if(virada == n * n){
        printf(GREEN_LIGHT "\n\nTodas Cartas viradas, fim de jogo!!\n\n"RESET);

        RegistrarRank(jogadoresAtuais,&Njogadores);

        jogAtuais += Njogadores; //Atualizar o total de jogadores que jogaram o jogo
        free(jogadoresAtuais);
        break;

        }

        // Variável para contar jogadores com vidas restantes
        int jogadoresComVidas = verificarvidas(n, jogadoresAtuais, Njogadores);

     //Verifica se existe ainda algum jogador que pode jogar o jogo
     if(jogadoresComVidas > 0){
     //Verificar se todas as vidas dos jogadores acabaram:
      for (int i = 0; i < Njogadores; i++) {
        atual = (atual + 1) % Njogadores;
       
        //verifica se o proximo jogador tem vidas disponiveis para continuar jogando:
        if (jogadoresAtuais[atual].vidas <= 0) {
            jogadoresComVidas--;  // Reduz o contador de jogadores com vidas restantes
            printf(RED"\nJogador %s nao tem mais vidas disponiveis! Passando pro proximo jogador!" RESET, jogadoresAtuais[atual].nome);
            pausar();
              
        //quebrar o loop e assim o proximo jogador com vidas disponiveis ira jogar
         } else{ break; }
     }
     }
     

     // Se todos os jogadores perderem as vidas
       if (jogadoresComVidas == 0) {
        printf(RED"\nTodos os jogadores perderam! Fim de jogo.\n"RESET);
        RegistrarRank(jogadoresAtuais,&Njogadores);
        jogAtuais += Njogadores; //Atualizar o total de jogadores que jogaram o jogo
        free(jogadoresAtuais);
        break;
        }

        system("clear || cls"); // Limpa a tela

    }
}

//Logicas do jogo:
 // Função para aleatorizar as peças:
 void Aleatorizar(cartas **Matriz, int N) {
    int total_elementos = N * N;
    int total_pares = total_elementos / 2;

    // Vetor para armazenar os pares (1 a total_pares)
    int numeros[total_elementos];

    // Preenche o vetor com os pares
    for (int i = 0; i < total_pares; i++) {
        numeros[i * 2] = i + 1; // exemplo: se torna numeros[2] = 1 + 1 = 2.
        numeros[i * 2 + 1] = i + 1; // exemplo: torna numeros[3] = 1 + 1 = 2.
    }

    // Embaralha o vetor de números
    for (int i = 0; i < total_elementos; i++) {
        int j = rand() % total_elementos;
        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }

    // Preenche a matriz com os números embaralhados
    int k = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Matriz[i][j].Carta_num = numeros[k++];
            Matriz[i][j].estado = 0;
            Matriz[i][j].estadotemp = 0;

        }
    }
}

//Opçoes de Tabuleiro
int Dificuldade() {
    int nivel;

    while(1) {
        system("clear || cls"); // Limpa a tela (usado no terminal)

        // Exibe as opções com bordas e cores
        printf(LILAC_LIGHT "Escolhendo o Nivel de Dificuldade\n");
        printf(PURPLE_DARK "-----------------------------------\n");
        printf(LILAC_DARK "1 - Facil\n");
        printf(LILAC_DARK "2 - Medio\n");
        printf(LILAC_DARK "3 - Dificil\n");
        printf(LILAC_DARK "4 - Sobre os niveis de dificuldade\n");
        printf(LILAC_DARK "-1 para retornar ao menu\n");
        printf(PURPLE_DARK "-----------------------------------\n");

        // Verificação para caso o usuario digite uma letra:
         printf("\nEscolha: ");
        if (scanf("%d", &nivel) != 1) {
            // Se a entrada não for um número, exibe uma mensagem e limpa o buffer
            printf(RED"Entrada invalida. Por favor, insira um numero inteiro.\n"RESET);
            while(getchar() != '\n'); // Limpa o buffer de entrada
            pausar();
            system("clear || cls"); // Limpa a tela
            continue; // Volta para o início do loop
        }

        getchar(); // Limpa o buffer

        switch (nivel) {
            case 1:
                system("clear || cls"); // Limpa a tela
                printf(LILAC_LIGHT "Nivel Facil\n");
                printf(LILAC_LIGHT "Matriz 4 x 4\n\n");
                return 4;
            case 2:
                system("clear || cls"); // Limpa a tela
                printf(LILAC_LIGHT "Nivel Medio\n");
                printf(LILAC_LIGHT "Matriz 6 x 6\n\n");
                return 6;
            case 3:
                system("clear || cls"); // Limpa a tela
                printf(LILAC_LIGHT "Nivel Dificil\n");
                printf(LILAC_LIGHT "Matriz 8 x 8\n\n");
                return 8;
            case 4:
                system("clear || cls"); // Limpa a tela
                niveis();
                pausar();
                system("clear || cls"); // Limpa a tela
                break;
            case -1:
                return -1;
            default:
                printf(LILAC_LIGHT "Opcao invalida, escolha uma opcao de 1 a 4 ou digite em -1 para voltar ao menu\n"RESET);
                pausar();
                system("clear || cls"); // Limpa a tela
        }
    }
}

//Função para Alocar a quantidade de jogadores desejados
jogador* jogadores(int *Njogador) {
    while (1) {
        printf(BLUE_LIGHT"\n\nDigite a quantidade de jogadores (Entre 1 a 3): "RESET);

        // Verificação para caso o usuario digite uma letra:
        if (scanf("%d", Njogador) != 1) {
            // Se a entrada não for um número, exibe uma mensagem e limpa o buffer
            printf(RED"Entrada invalida. Por favor, insira um numero inteiro.\n"RESET);
            while(getchar() != '\n'); // Limpa o buffer de entrada
            pausar();
            system("clear || cls"); // Limpa a tela
            continue; // Volta para o início do loop
        }

        //Vericação limite de jogadores ou numeros negativos:
        if (*Njogador > 3 || *Njogador <= 0) {
            printf(RED"\nNúmero inválido, escolha um número entre 1 a 3!"RESET);
            pausar();
            getchar(); // Aguarda a entrada do usuário
            system("clear || cls"); // Limpa a tela
        } else {
            break;
        }
    }

    jogador *jogadoresAtuais = (jogador*) malloc(*Njogador * sizeof(jogador));

    if (jogadoresAtuais == NULL) {
        printf(RED"Falha ao alocar memória para os jogadores.\n"RESET);
        return NULL;
    }

    getchar(); // Limpa o buffer após scanf
    for (int i = 0; i < *Njogador; i++) {
        while (1) {
            printf("Nome do %d jogador: ", i + 1);
            fgets(jogadoresAtuais[i].nome, sizeof(jogadoresAtuais[i].nome), stdin);

            // Remover o '\n' do final do nome
            jogadoresAtuais[i].nome[strcspn(jogadoresAtuais[i].nome, "\n")] = '\0';

            if (strlen(jogadoresAtuais[i].nome) > 0) {
                break;
            }
            printf(RED"Nome inválido. Digite novamente.\n"RESET);
        }
    }
    for(int i = 0; i < *Njogador;i++){
        jogadoresAtuais[i].pontuacao = 0;
    }

    return jogadoresAtuais;
}

// Fumção para a escolha da posição das cartas e validação das escolhas
int escolha(int *lin1, int *col1, int *lin2, int *col2, int n, cartas **Matriz, jogador *jogadoresatuais, int atual){

    // Leitura e validação das posições da 1 carta:
    while(1){
     printf( PURPLE_LIGHT"\nDigite a posicao (linha e coluna) da 1 carta: "RESET);

      // Verificação para caso o usuario digite uma letra:
        if (scanf("%d %d", lin1, col1) != 2) {
            // Se a entrada não for um número, exibe uma mensagem e limpa o buffer
            printf(RED"Entrada invalida. Por favor, insira um numero inteiro.\n"RESET);
            while(getchar() != '\n'); // Limpa o buffer de entrada
            continue; // Volta para o início do loop
        }

     // validar para ve se as entradas estao dentro do limite da matriz:
     if (*lin1 < 0 || *lin1 >= n || *col1 < 0 || *col1 >= n) {
        // Validar se o jogador quer sair
        if (*lin1 == -1 && *col1 == -1) {
            printf(RED"\nUsuario(a) %s desistiu da partida, jogo finalizado!\n"RESET, jogadoresatuais[atual].nome);
            return -1;
        }

        if(*lin1 == -2 && *col1 == -2){
            coringa(Matriz,n,&atual,jogadoresatuais);
            MostrarMatriz(Matriz, n);
            exibirjogador(jogadoresatuais, &atual);
            continue;
        }

        printf(RED"\nCoordenadas invalidas! Digite valores entre 0 e %d para linha e coluna.\n"RESET, n - 1);
        continue;
    }


     //verificações:
     if(Matriz[*lin1][*col1].estadotemp == 1 || Matriz[*lin1][*col1].estado == 1){
        printf(RED"\nposicao ja foi escolhida, tente outra!"RESET);
        continue;
     }
        break;

    }

    Matriz[*lin1][*col1].estadotemp = 1;

    system("clear || cls"); // Limpa a tela
    MostrarMatriz(Matriz, n);
    exibirjogador(jogadoresatuais, &atual);

    // Leitura e validação das posições da 2 carta:
    while(1){

     printf( PURPLE_LIGHT"\nDigite a posicao (linha e coluna) da 2 carta: "RESET);

     // Verificação para caso o usuario digite uma letra:
        if (scanf("%d %d", lin2, col2) != 2) {
            // Se a entrada não for um número, exibe uma mensagem e limpa o buffer
            printf(RED"Entrada invalida. Por favor, insira um numero inteiro.\n"RESET);
            while(getchar() != '\n'); // Limpa o buffer de entrada
            continue; // Volta para o início do loop
        }

     // validar para ve se as entradas estao dentro do limite da matriz:
     if (*lin2 < 0 || *lin2 >= n || *col2 < 0 || *col2 >= n) {
        // Validar se o jogador quer sair
        if (*lin2 == -1 && *col2 == -1) {
            printf(RED"\nUsuario(a) %s desistiu da partida, jogo finalizado!\n"RESET, jogadoresatuais[atual].nome);
            return -1;
        }
        if(*lin2 == -2 && *col2 == -2){
            coringa(Matriz,n,&atual,jogadoresatuais);
            MostrarMatriz(Matriz, n);
            exibirjogador(jogadoresatuais, &atual);
            continue;
        }

        printf(RED"\nCoordenadas invalidas! Digite valores entre 0 e %d para linha e coluna.\n" RESET, n - 1);
        continue;
    }


     //verificações:
     if(Matriz[*lin2][*col2].estadotemp == 1 || Matriz[*lin2][*col2].estado == 1){
        printf(RED"\nposicao ja foi escolhida, tente outra!"RESET);
        continue;
     }
        break;

    }

    Matriz[*lin2][*col2].estadotemp = 1;

    //significa que o jogo vai continuar:
    return 0;

}

// Função para validar os pares escolhidos
void Pares(cartas **matriz, int *lin1, int *col1, int *lin2, int *col2,int *viradas, jogador *jogadoresatuais, int atual){

    //comparação se os valores sao iguais:
    if(matriz[*lin1][*col1].Carta_num == matriz[*lin2][*col2].Carta_num){

        printf(RED"Acertouu!"RESET);  // Exibe a mensagem na posição inicial
        sleep(2);  // Espera 2 segundo

        //adiciona um ponto ao jogador da vez:
        jogadoresatuais[atual].pontuacao += 1;

        //define as variaveis temporaria a 0, e o estado fixo em 1:
        matriz[*lin1][*col1].estado = 1;
        matriz[*lin2][*col2].estado = 1;
        matriz[*lin1][*col1].estadotemp = 0;
        matriz[*lin2][*col2].estadotemp = 0;

        *viradas +=2; // toda vez que acerta duas cartas vao ser desviradas, entao mais dois.


    } else{
        printf(RED"\nErro!\n"RESET);
        sleep(2);  // Pausa por 2 segundos
        jogadoresatuais[atual].vidas -=1;
       
        matriz[*lin1][*col1].estado = 0;
        matriz[*lin2][*col2].estado = 0;
        matriz[*lin1][*col1].estadotemp = 0;
        matriz[*lin2][*col2].estadotemp = 0;
    }
}



//Funções Especiais(adicionais do jogo da memoria)
 // Função para verificar as chances coringas dos jogadores:
void coringa(cartas **matriz, int n, int *atual, jogador *jogadoresatuais){

    if(jogadoresatuais[*atual].chancesCoringas > 0){
       printf(RED"\n\nRecurso Chance coringa ativada!\n"RESET);
       pausar();
       system("clear || cls");
       mostrarCoringa(matriz,n);
       sleep(5);
       jogadoresatuais[*atual].chancesCoringas  -= 1;

     } else{
        printf(RED"Voce nao tem disponivel chances coringas!"RESET);
        pausar();
        system("clear || cls");
    }

    system("clear || cls");

}

//Função para o inicio do jogo, saber o tamanho da dificuldade e assim quantas chances coringas estarao disponiveis para os jogadores
 void chanceCoringa(int n, jogador *jogadoresAtuais, int Njogadores){
    switch(n){
        case 4:{
            if(Njogadores == 1){
                jogadoresAtuais[0].chancesCoringas = 2;

            } else{
                for(int i = 0; i < Njogadores; i++){
                jogadoresAtuais[i].chancesCoringas = 1;
                }
            }

            break;
        }
        case 6:{
             if(Njogadores == 1){
                jogadoresAtuais[0].chancesCoringas = 3;

            } else{
                for(int i = 0; i < Njogadores; i++){
                jogadoresAtuais[i].chancesCoringas = 2;
                }
            }

            break;
        }
        case 8:{
            if(Njogadores == 1){
                jogadoresAtuais[0].chancesCoringas = 6;

            } else{
                for(int i = 0; i < Njogadores; i++){
                jogadoresAtuais[i].chancesCoringas = 3;
                }
            }

            break;
        }

    }
}

//Funçaõ que destribuir as vidas para cada jogador:
 void chanceVidas(int n, jogador *jogadoresAtuais, int Njogadores){
    switch(n){
        case 4:{
            if(Njogadores == 1){
               jogadoresAtuais[0].vidas = 4;
            } else{
                for(int i = 0; i < Njogadores; i++){
                jogadoresAtuais[i].vidas = 2;
            }
            }
            
            break;
        }
        case 6:{
            if(Njogadores == 1){
               jogadoresAtuais[0].vidas = 8;
            } else{
                for(int i = 0; i < Njogadores; i++){
                jogadoresAtuais[i].vidas = 4;
            }
            }
            
            break;
        }
        case 8:{
            if(Njogadores == 1){
               jogadoresAtuais[0].vidas = 12;
            } else{
                for(int i = 0; i < Njogadores; i++){
                jogadoresAtuais[i].vidas = 6;
            }
            }
            
            break;
        }

    }
}

//Função que faz a contagem para ve se todos os jogadores perderam as vidas:
int verificarvidas(int n, jogador *jogadoresAtuais, int Njogadores){

    int temp = Njogadores; //variavel para armezena quantos jogadores estao sem vida

    //Verificar se todas as vidas dos jogadores acabaram:
      for (int i = 0; i < Njogadores; i++) {
        if (jogadoresAtuais[i].vidas <= 0) {
            temp--;
              
        //quebrar o loop e assim o proximo jogador com vidas disponiveis ira jogar
         } else{ 
            return Njogadores; }
     }

     return 0;

}

// funções ilustrativas:
// mostrar o menu do jogo:
void mostrarMenu() {
    printf(" "RESET);
    int i, j;
    char menu[5][50] = {
        "1 - Jogar",
        "2 - Regras",
        "3 - Ranking",
        "4 - Creditos",
        "5 - Sair"
    };
    char vtTela[25][50];

    // Preencher a tela com espaços
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 50; j++) {
            vtTela[i][j] = '.';  // Usando ponto para o fundo
        }
    }

    // Criar a borda com o caractere '#'
    for (j = 0; j < 50; j++) {
        vtTela[0][j] = '#';  // Linha superior da borda
        vtTela[24][j] = '#'; // Linha inferior da borda
    }
    for (i = 0; i < 25; i++) {
        vtTela[i][0] = '#';  // Coluna da borda esquerda
        vtTela[i][49] = '#'; // Coluna da borda direita
    }

    // Colocar o menu na tela
    for (i = 0; i < 5; i++) {
        for (j = 0; menu[i][j] != '\0'; j++) {
            vtTela[i + 10][j + 12] = menu[i][j];  // Posições para o menu
        }
    }

    // Exibir a tela com cores
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 50; j++) {
            if (vtTela[i][j] == '#') {
                // Pintar as bordas com roxo escuro
                printf(PURPLE_DARK "%c" RESET, vtTela[i][j]);
            } else if (i >= 10 && i < 15 && vtTela[i][j] != '.') {
                // Pintar o menu com lilás claro (para todas as opções)
                printf(LILAC_LIGHT "%c" RESET, vtTela[i][j]);
            } else {
                // Imprimir o fundo (pontos) com lilás escuro suave
                printf(LILAC_DARK "%c" RESET, vtTela[i][j]);
            }
        }
        printf("\n");
    }
}

//Mostrar o Tabuleiro do jogo:
 void MostrarMatriz(cartas **Matriz, int N) {
    // Exibição de boas-vindas com borda decorativa
    printf(BLUE_CYAN"\n========================================================\n"RESET);
    printf( BLUE_DARK  "                     JOGO DA MEMORIA\n"RESET);
    printf(BLUE_CYAN"========================================================\n\n"RESET);

    // Imprimir borda superior
    printf("    +");
    for (int j = 0; j < N; j++) {
        printf("-------+");
    }
    printf("\n");

    // Imprimir a matriz com números das linhas e conteúdo alinhado
    for (int i = 0; i < N; i++) {
        printf("%3d |", i);  // Exibe o número da linha alinhado
        for (int j = 0; j < N; j++) {
            if (Matriz[i][j].estadotemp == 1) {
                // Carta temporariamente revelada em vermelho
                printf(" \033[1;31m%3d\033[0m  |", Matriz[i][j].Carta_num);
            } else if (Matriz[i][j].estado == 0) {
                // Carta escondidas
                printf(" \033[1;36m ** \033[0m |");
            } else {
                // Carta revelada permanentemente sem cor
                printf(" %3d  |", Matriz[i][j].Carta_num);
            }
        }
        printf("\n");

        // Imprimir linha entre as células
        printf("    +");
        for (int j = 0; j < N; j++) {
            printf("-------+");
        }
        printf("\n");
    }

    // Imprimir números das colunas abaixo da matriz
    printf("     ");
    for (int j = 0; j < N; j++) {
        printf("   %d   ", j);
    }
    printf("\n");
    printf(BLUE_CYAN"\n========================================================\n"RESET);
}

// Função para quando a opção -2 -2, for ativada mostrar o tabuleiro e as cartas aparecem todas desviradas por 5 segundos
 void mostrarCoringa(cartas **Matriz, int N) {
    // Exibição de boas-vindas com borda decorativa
    printf("\n========================================================\n");
    printf("                     JOGO DA MEMORIA\n");
    printf("========================================================\n\n");

    // Imprimir borda superior
    printf("    +");
    for (int j = 0; j < N; j++) {
        printf("-------+");
    }
    printf("\n");

    // Imprimir a matriz com números das linhas e conteúdo alinhado
    for (int i = 0; i < N; i++) {
        printf("%3d |", i);  // Exibe o número da linha alinhado
        for (int j = 0; j < N; j++) {
            if (Matriz[i][j].estadotemp == 1) {
                // Carta temporariamente revelada em vermelho
                printf(" \033[1;31m%3d\033[0m  |", Matriz[i][j].Carta_num);

            } else {
                // Carta revelada permanentemente sem cor
                printf(" %3d  |", Matriz[i][j].Carta_num);
            }
        }
        printf("\n");

        // Imprimir linha entre as células
        printf("    +");
        for (int j = 0; j < N; j++) {
            printf("-------+");
        }
        printf("\n");
    }

    // Imprimir números das colunas abaixo da matriz
    printf("     ");
    for (int j = 0; j < N; j++) {
        printf("   %d   ", j);
    }
    printf("\n");
    printf("\n========================================================\n");
}

//mostrar o nome do jogador, sua atual pontuação e quantas chances coringas ele tem disponivel:
 void exibirjogador(jogador *jogadoresatuais, int *atual){

    printf(GREEN_LIGHT "\n\nJogador da vez: %s", jogadoresatuais[*atual].nome);
    printf("\npontuacao: %i", jogadoresatuais[*atual].pontuacao);
    printf("\nChance coringa: %i", jogadoresatuais[*atual].chancesCoringas);
    printf("\nVidas: %i\n"RESET, jogadoresatuais[*atual].vidas);
    printf(BLUE_LIGHT "OBS: Para desistir, digite -1 -1. Para ativar o coringa, digite -2 -2.\n\n" RESET);


}

//Mostrar as regras do jogo e como funciona:
void regras() {
    //Titulo
    printf(BLUE_LIGHT "\n========================================================\n");
    printf(GREEN_LIGHT "                     " LILAC_LIGHT "REGRAS" RESET "\n");
    printf(BLUE_LIGHT "========================================================\n\n");

    // Objetivo do Jogo
    printf(LILAC_LIGHT "- Objetivo do Jogo:\n");
    printf(BLUE_CYAN "    - Encontrar todos os pares de cartas correspondentes.\n\n");

    // Estrutura do Jogo
    printf(LILAC_LIGHT "- Estrutura do Jogo:\n");
    printf(BLUE_CYAN  "    - O tabuleiro tera 16 cartas dispostas em um grid de 4x4.\n");
    printf(BLUE_CYAN  "    - Cada carta tera uma imagem ou simbolo que sera revelado quando a carta for virada.\n\n");

    // Configuracao Inicial
    printf(LILAC_LIGHT "- Configuracao Inicial:\n");
    printf(BLUE_CYAN  "    - 8 pares de cartas identicas (16 cartas no total).\n");
    printf(BLUE_CYAN  "    - As cartas serao embaralhadas e distribuidas aleatoriamente no tabuleiro.\n\n");

    // Como Jogar
    printf(LILAC_LIGHT "- Como Jogar:\n");
    printf(BLUE_CYAN "    - Os dois jogadores se revezam para escolher duas cartas.\n");
    printf(BLUE_CYAN  "    - Ao selecionar uma carta, ela e virada para mostrar sua face.\n");
    printf(BLUE_CYAN  "    - Se a segunda carta escolhida corresponder a primeira, o jogador mantem o par e ganha um ponto.\n");
    printf(BLUE_CYAN  "    - Se as cartas nao corresponderem, ambas sao viradas de volta apos um breve intervalo, permitindo que os jogadores memorizem a posicao.\n\n");

    // Desistencia
    printf(LILAC_LIGHT "- Desistencia:\n");
    printf(BLUE_CYAN  "    - Se o jogador quiser desistir, ele pode apertar -1 -1 ao escolher a coluna e linha da carta.\n");
    printf(BLUE_CYAN  "    - Isso ira encerrar sua participacao no jogo.\n\n");

    // O Coringa
    printf(LILAC_LIGHT "- O Coringa:\n");
    printf(BLUE_CYAN  "    - O coringa oferece uma chance especial de ver o tabuleiro inteiro desvirado por 5 segundos.\n");
    printf(BLUE_CYAN "    - Durante esse tempo, o jogador deve memorizar as posicoes das cartas.\n");
    printf(BLUE_CYAN  "    - As chances de coringa sao limitadas e dependem do nivel do jogo (veja abaixo).\n\n");

    // Niveis do Jogo
    printf(LILAC_LIGHT "- Niveis do Jogo:\n");
    printf(BLUE_CYAN "    - Nivel Facil: Matriz 4x4, 1 chance coringa por jogador.\n");
    printf(BLUE_CYAN  "    - Nivel Medio: Matriz 6x6, 2 chances coringa por jogador.\n");
    printf(BLUE_CYAN  "    - Nivel Dificil: Matriz 8x8, 4 chances coringa por jogador.\n\n");

    // Fim do Jogo
    printf(LILAC_LIGHT "- Fim do Jogo:\n");
    printf(BLUE_CYAN  "    - O jogo termina quando todos os pares forem encontrados ou quando um jogador decidir sair.\n");
    printf(BLUE_CYAN  "    - O jogador com o maior numero de pares (pontos) vence.\n" RESET);
}

// Função que imprime um texto caractere por caractere com um atraso de 1s
void imprimirComDelay(const char* texto, int delay) {
    while (*texto) {
        putchar(*texto);
        fflush(stdout);  // Garante que o caractere seja impresso imediatamente
        Sleep(delay);   // Atraso em microssegundos (1 segundo = 1000000 microssegundos)
        texto++;
    }
}

// Função para exibir os créditos do jogo com efeito de digitação
void creditos() {
    printf(BLUE_LIGHT "\n========================================================\n");
    imprimirComDelay("                     " LILAC_LIGHT "CREDITOS\n", 1); // Delay de 0.1 segundo por caractere
    printf(BLUE_LIGHT "========================================================\n\n");

    imprimirComDelay(LILAC_LIGHT "--- Trabalho Final de LP1 --- ", 1);
    imprimirComDelay(PURPLE_DARK "\nAluna: Ana Paula\n", 1);
    imprimirComDelay(LILAC_LIGHT "Professor: Grimaldo\n", 1);
    imprimirComDelay(PURPLE_DARK "Inspirado no trabalho de Luiz Gustavo Nunes,\n", 1);
    imprimirComDelay(PURPLE_DARK "realizado no primeiro ano do curso de Engenharia de Software da UEPG.\n", 1);
    printf(RESET "\n");
}

//Mostrar como funciona os niveis do jogo desde o facil ao mais dificil
void niveis() {
     printf(PURPLE_DARK "\n========================================================\n"RESET);
    printf(GREEN_LIGHT"                       Niveis\n"RESET);
    printf(PURPLE_DARK "========================================================\n\n"RESET);

    printf(LILAC_LIGHT "Como funciona os niveis de dificuldades?\n\n"RESET);

    // Nível fácil
    printf(LILAC_DARK "Nivel Facil:\n" RESET);
    printf("Matriz 4x4, 1 chance coringa por jogador.\n");
    printf("Este nivel oferece uma pequena matriz com menos pares de cartas.\n");
    printf("As chances coringa ajudam a pular uma jogada ou a revelar uma carta sem erro.\n\n");

    // Nível médio
    printf(LILAC_DARK "Nivel Medio:\n" RESET);
    printf("Matriz 6x6, 2 chances coringa por jogador.\n");
    printf("Aqui, a matriz aumenta em tamanho, com mais pares de cartas.\n");
    printf("Os jogadores podem contar com duas chances coringa para auxiliar na jogabilidade.\n\n");

    // Nível difícil
    printf(PURPLE_LIGHT "Nivel Dificil:\n" RESET);
    printf("Matriz 8x8, 4 chances coringa por jogador.\n");
    printf("Neste nivel, a matriz e grande, com muitos pares de cartas a serem combinados.\n");
    printf("O número de chances coringa e maior, permitindo mais flexibilidade, mas a dificuldade tambem aumenta.\n");
}



// Funções basicas:
   //liberar a matriz quando o jogo acabar ou quando o jogador quiser finalizar o jogo:
void liberarMatriz(cartas **matriz, int n){
    for(int i = 0; i < n; i++){
        free(matriz[i]);
    }
    free(matriz);
}

//Pausa o jogo e da tempo pro usuario le a tela antes de mudar:
void pausar() {
    printf(LILAC_LIGHT "\n");
    system("pause");
    printf(" "RESET);
}




//Função para Rank:
  //Criar o arquivo pro rank
void CriarArquivo(){
   Arank = fopen("rank1.txt", "a"); // Abrir o arquivo para adicionar os dados

    if (Arank == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    fclose(Arank); // Fecha o arquivo após criar
}

 //Função que registrar no arquivo os dados dos jogadores
  void RegistrarRank(jogador *atuaisjogadores, int *numJogadores){

    Arank = fopen("rank1.bin", "ab");  // Abre o arquivo para escrita binária
    if (Arank == NULL) {
        printf("Erro ao abrir o arquivo para salvar jogadores.\n");
        return;
    }

    fwrite(atuaisjogadores, sizeof(jogador), *numJogadores, Arank);
    fclose(Arank);  // Fecha o arquivo após salvar

}

// Mostrar o rank:
  void rank(){
    //Declaração do ponteiro que vai receber o vetor com os jogadores:
    jogador *jogadoresatuais = NULL;

    printf(PURPLE_DARK "\n========================================================\n");
    printf("                       RANKING\n");
    printf("========================================================\n\n"RESET);
    printf("\n\nTotal de Jogadores: %d\n\n", jogAtuais);
    if(jogAtuais > 0){
        jogadoresatuais = OrdemRank();

        if (jogadoresatuais == NULL) { // Verifica se houve erro na função OrdemRank
            printf("Erro ao carregar os jogadores.\n");
            return;
        }

   // Imprime o array dos jogadores do arquivo
        for (int i = 0; i < jogAtuais; i++) {
             printf("\t %d- Nome: %s, Pontuacao: %d\n", i+1, jogadoresatuais[i].nome, jogadoresatuais[i].pontuacao);
     }

    } else {
        printf(RED"\nNenhum jogador registrado ainda!\n"RESET);
    }

     free(jogadoresatuais); // Libera a memória alocada

}

jogador *OrdemRank(){
    
    //alocar um vetor para armazenar os jogadores
    jogador *jogadores = (jogador *)malloc(jogAtuais * sizeof(jogador));
    if (jogadores == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    int numJogadores = 0;

      // Reabre o arquivo apenas para leitura
    Arank = fopen("rank1.bin", "rb");
    if (Arank == NULL) {
        printf(RED "Erro ao abrir o arquivo para leitura.\n" RESET);
        free(jogadores);
        return NULL;
    }

    // Lê os jogadores do arquivo
    while (numJogadores < jogAtuais && fread(&jogadores[numJogadores], sizeof(jogador), 1, Arank) == 1) {
        numJogadores++;
    }

    fclose(Arank);

        // Ordena os jogadores por pontuação (ordem decrescente)
        jogador temp;
        for (int i = 0; i < numJogadores - 1; i++) {
            for (int j = 0; j < numJogadores - i - 1; j++) {
                if (jogadores[j].pontuacao < jogadores[j + 1].pontuacao) {
                    // Troca os jogadores
                    temp = jogadores[j];
                    jogadores[j] = jogadores[j + 1];
                    jogadores[j + 1] = temp;
                }
            }
        }

    return jogadores;
}


//Arquivo jogadores Atuais
 // Criar um arquivo para armazena o numero de jogadores que jogaram o jogo:
FILE *ArqAtuais(){
    FILE *ArqJog = fopen("jogadoresatuais.txt", "a"); // Abrir o arquivo para adicionar os dados

    if (ArqJog== NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }


    return ArqJog;
}

// Função atualiza(Atribuir) o jogAtuais no inicio do jogo
 void Atuais(FILE *ArqJog){

    ArqJog = fopen("jogadoresatuais.txt", "r"); // Reabre o arquivo apenas para leitura
    fscanf(ArqJog, "%d", &jogAtuais); // Lê o valor de jogAtuais do arquivo e armazena
    fclose(ArqJog); 
    
 }

// Toda vez que o usuario fechar o menu, vai salvar a quantidade atual de jogadores
 void EscreverAtuais(FILE *ArqJog){
    ArqJog = fopen("jogadoresatuais.txt", "w");  // Abre o arquivo para sobrescrever os dados
    if (ArqJog == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(ArqJog, "%d", jogAtuais); //Escreve no arquivo a quantidade de jogadores que ja jogaram o jogo
    fclose(ArqJog);  // Fecha o arquivo após a gravação
 }
