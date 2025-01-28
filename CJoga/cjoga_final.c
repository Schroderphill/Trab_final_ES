#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // Para usleep
#ifdef _WIN32
#include <conio.h> // Para kbhit e getch no Windows
#else
#include <termios.h> // Para kbhit no Unix/Linux
#include <fcntl.h> // Para kbhit no Unix/Linux
#endif

#define TAM 3
#define MAX_TENTATIVAS 6
#define TAM_PALAVRA 30
#define LARGURA 40  //Define a largura do campo da cobra
#define ALTURA 20   //Define a altura do campo da cobra

int pts = 0;    //Armazena a pontuação do usuário em tempo real
int perdeu = 0; //Termina o jogo quando muda o seu valor para 0

// Estruturas para o Jogo da Cobrinha
//Struct que indica posições, será usada como uma struct aninhada durante o código
typedef struct {
    int x, y;
} Posicao;

//Struct da cobra em si
typedef struct {
    Posicao corpo[100]; //Array contendo as partes da cobra
    int comprimento;    //Int indicando o tamanho atual da cobra
    char direcao;   //Direção em que a cobra ta seguindo no momento(D, E, C, B; Direita, Esquerda, Cima e Baixo)
} Cobra;

//Struct da comida da cobra
typedef struct {
    Posicao pos;    //Indica a posição no campo da comida(struct aninhada para a comida)
} Comida;

Cobra cobra;    //Declaração da cobra
Comida comida;  //Declaração da comida

// Declaracoees das funcoes dos jogos
void jogoDaVelha();
void jogoDaForca();
void inicializarTabuleiro(char tabuleiro[TAM][TAM]);
void exibirTabuleiro(char tabuleiro[TAM][TAM]);
char verificarVencedor(char tabuleiro[TAM][TAM]);
int tabuleiroCheio(char tabuleiro[TAM][TAM]);
void desenharForca(int erros);
void esconderPalavra(char* palavra, char* palavraOculta);
int venceu(char* palavraOculta);
int kbhit(void);

// Fun��o para exibir o menu inicial
void exibirMenu() {
    printf("\n");
    printf("************************************\n");
    printf("*           C  J O G A             *\n");
    printf("************************************\n");
    printf("\nSelecione uma das opcoes:\n");
    printf("01 - Jogo da Velha\n");
    printf("02 - Jogo da Cobrinha\n");
    printf("03 - Jogo da Forca\n");
    printf("04 - Sair\n");
    printf("Digite sua escolha: ");
}

int main() {
    int opcao;
    srand(time(NULL)); // Inicializar a semente do rand uma vez

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogoDaVelha();
                break;
            case 2:
                iniciarCobra();

        while (!perdeu) {
            desenhar();
            input();
            logicaCobra();
            if (cobra.direcao == 'C' || cobra.direcao == 'B') { //Verifica se a cobra está indo para cima ou baixo, e em caso positivo aumenta o delay para o print e fazer a velocidade da cobra parecer unanime
        Sleep(200);
        } else {
            Sleep(100); //Delay caso a cobra esteja indo para a direita ou esquerda
            }
        }

    printf("Fim de jogo. Sua pontuacao: %d\n", pts);
                break;

            case 3:
                jogoDaForca();
                break;
            case 4:
                printf("\nSaindo do aplicativo... Obrigado por jogar!\n");
                break;
            default:
                printf("\n Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}

// Implementacao do Jogo da Velha -------------------------------------------------------------------------------------
void jogoDaVelha() {
    char tabuleiro[TAM][TAM];
    int linha, coluna;
    char jogadorAtual = 'X';
    char vencedor = ' ';
    int jogadas = 0;

    inicializarTabuleiro(tabuleiro);

    printf("Bem-vindo ao Jogo da Velha!\n");

    while (vencedor == ' ' && !tabuleiroCheio(tabuleiro)) {
        exibirTabuleiro(tabuleiro);
        printf("Jogador %c, � sua vez!\n", jogadorAtual);

        // Entrada da posicao
        printf("Digite a linha (0, 1 ou 2) ou 'q' para voltar ao menu: ");
        char input;
        scanf(" %c", &input);

        if (input == 'q' || input == 'Q') {
            printf("\nVoltando ao menu...\n");
            return;
        }

        linha = input - '0';
        printf("Digite a coluna (0, 1 ou 2): ");
        scanf("%d", &coluna);

        // Verificar se a posicao e valida
        if (linha < 0 || linha >= TAM || coluna < 0 || coluna >= TAM || tabuleiro[linha][coluna] != ' ') {
            printf("Jogada invalida! Tente novamente.\n");
            continue;
        }

        // Atualizar o tabuleiro com a jogada
        tabuleiro[linha][coluna] = jogadorAtual;
        jogadas++;

        // Verificar se ha um vencedor
        vencedor = verificarVencedor(tabuleiro);

        // Alternar jogador
        jogadorAtual = (jogadorAtual == 'X') ? 'O' : 'X';
    }

    exibirTabuleiro(tabuleiro);

    if (vencedor != ' ') {
        printf("Parabens! O jogador %c venceu!\n", vencedor);
    } else {
        printf("Empate! O tabuleiro esta cheio.\n");
    }
}

void inicializarTabuleiro(char tabuleiro[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
}

void exibirTabuleiro(char tabuleiro[TAM][TAM]) {
    printf("\n");
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < TAM - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (i < TAM - 1) {
            printf("---+---+---\n");
        }
    }
    printf("\n");
}

char verificarVencedor(char tabuleiro[TAM][TAM]) {
    // Verificar linhas
    for (int i = 0; i < TAM; i++) {
        if (tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2] && tabuleiro[i][0] != ' ') {
            return tabuleiro[i][0];
        }
    }

    // Verificar colunas
    for (int i = 0; i < TAM; i++) {
        if (tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i] && tabuleiro[0][i] != ' ') {
            return tabuleiro[0][i];
        }
    }

    // Verificar diagonais
    if (tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2] && tabuleiro[0][0] != ' ') {
        return tabuleiro[0][0];
    }
    if (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0] && tabuleiro[0][2] != ' ') {
        return tabuleiro[0][2];
    }

    return ' '; // Nenhum vencedor
}

int tabuleiroCheio(char tabuleiro[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == ' ') {
                return 0;
            }
        }
    }
    return 1;
}

// Implementacao do Jogo da Cobrinha ----------------------------------------------------------------------------------------------

//Função para iniciar a cobra
void iniciarCobra() {

    cobra.comprimento = 1;  //Define o tamanho inicial da cobra em 1
    cobra.corpo[0].x = LARGURA / 2; //Esse e o de baixo colocam a posição inicial da cobra no meio do campo
    cobra.corpo[0].y = ALTURA / 2;
    cobra.direcao = 'D';            //Começa movendo a cobra para a direita

    //Função para inicializar a comida da cobra
    srand(time(0)); //Inicializa o srand
    comida.pos.x = rand() % LARGURA;    //Define aleatoriamente a localização horizontal(eixo x) da comida da cobra
    comida.pos.y = rand() % ALTURA;     //Define aleatoriamente a localização vertical(eixo y) da comida da cobra

    pts = 0;    //Começa/reinicia a pontuação no zero
    perdeu = 0; //Coloca o perdeu(valor que mantém o jogo rodando) em 0 (falso)
}

//Função para desenhar as coisas do jogo
void desenhar() {
    system("cls");  //Limpa o terminal cada vez que a função for usada

    //Desenha a borda superior
    for (int i = 0; i < LARGURA + 2; i++) {
        printf("#");
        }
    printf("\n");


    for (int i = 0; i < ALTURA; i++) {  //Loop para passar por todas as linhas
        for (int j = 0; j < LARGURA; j++) { //Loop para passar por todas as colunas
            if (j == 0) printf("#");        //Printar a parede esquerda quando a coluna for 0(primeira coluna do terminal)

            //Desenha a cobra
            int posicaoOcupada = 0;  //Para verficar se a posição é coupada pela comida ou cobra
            for (int l = 0; l < cobra.comprimento; l++) {   //Verifica se a posição que ta verificando faz parte do corpo da
                if (cobra.corpo[l].x == j && cobra.corpo[l].y == i) { //<-- Ira printar uma parte do corpo da cobra se a poisção
                    printf("O");                                      //x e y(horizontal e vertifal) for parte do corpo da cobra
                    posicaoOcupada = 1;                                    //atualmente
                    break;
                }
            }

            // desenhar comida
            if (comida.pos.x == j && comida.pos.y == i) {   //Checa se a posição que ta verficiando é ocupada por uma comida
                printf("X");    //Printa a comida
                posicaoOcupada = 1;
            }

            if (!posicaoOcupada) printf(" ");   //Se a posição não for coupada por nada "desenha" espaço

            if (j == LARGURA - 1) printf("#"); //Desenha a parede direita
        }
        printf("\n");
    }

    //Desenha a borda inferior
    for (int i = 0; i < LARGURA + 2; i++) printf("#");
    printf("\n");

    //Mostra a pontuação atual do jogador
    printf("Pontuação: %d\n", pts);
}

//Funão para aceitar os inputs do teclado do usuário
void input() {
    if (_kbhit()) { //Verifica se alguma tecla foi pressionada
        char tecla = _getch();  //Registra a tecla que foi pressionada pelo usuário
        switch (tecla) {    //Switch case para verificar se a tecla pressionada é alguma que movimenta ou sai do jogo
            case 'a':
                if (cobra.direcao != 'D') cobra.direcao = 'E';  //Move para esquerda se não estiver se movendo para a direita
                break;
            case 'd':
                if (cobra.direcao != 'E') cobra.direcao = 'D';  //Move para direita se não estiver se movendo para a esquerda
                break;
            case 'w':
                if (cobra.direcao != 'B') cobra.direcao = 'C';  //Move para cima se não estiver se movendo para baixo
                break;
            case 's':
                if (cobra.direcao != 'C') cobra.direcao = 'B';  //Move para baixo se não estiver se movendo para cima
                break;
            case 'q':
                perdeu = 1; //Termina o jogo ao clicar q
                break;
        }
    }
}

//Função para a lógica do jogo da cobra
void logicaCobra() {

    Posicao ant = cobra.corpo[0];   //Aramzena a posição da cabeça da cobra
    Posicao ant2;

    switch (cobra.direcao) {    //Atualiza o movimento da cobra com base na direção(como um plano cartesiano)
        case 'E':
            cobra.corpo[0].x--;
            break;
        case 'D':
            cobra.corpo[0].x++;
            break;
        case 'C':
            cobra.corpo[0].y--;
            break;
        case 'B':
            cobra.corpo[0].y++;
            break;
    }

    //Verifica se a cobra se bateu em alguma parede
    if (cobra.corpo[0].x < 0 || cobra.corpo[0].x >= LARGURA || cobra.corpo[0].y < 0 || cobra.corpo[0].y >= ALTURA) {
        perdeu = 1;
    }

    //Verifica se a cobra se bateu no próprio corpo
    for (int i = 1; i < cobra.comprimento; i++) {   //Loop para verificar se a cabeça da cobra ocupa a mesma posição de alguma parte do corpo
        if (cobra.corpo[0].x == cobra.corpo[i].x && cobra.corpo[0].y == cobra.corpo[i].y) {
            perdeu = 1;
        }
    }

    //Verifica se a cobra comeu a comida
    if (cobra.corpo[0].x == comida.pos.x && cobra.corpo[0].y == comida.pos.y) { //Verifica se a cabeça da cobra e a comida ocupam a mesma posição
        pts++;  //Aumenta a pontuação
        cobra.comprimento++;    //Aumenta o comprimento
        comida.pos.x = rand() % LARGURA;    //Gera a posição horizontal da comida
        comida.pos.y = rand() % ALTURA;     //gera a posição vertical da comida
    } else {
        //Move o corpo da cobra
        for (int i = 1; i < cobra.comprimento; i++) {
            ant2 = cobra.corpo[i];
            cobra.corpo[i] = ant;   //Sort para trocar a posição da cobra
            ant = ant2;
        }
    }
}

// Implementacao do Jogo da Forca ------------------------------------------------------------------------------------------
void jogoDaForca() {
    char palavra[TAM_PALAVRA], palavraOculta[TAM_PALAVRA], tentativa;
    char tentativas[26] = {0};
    int numTentativas = 0, acertos = 0, erros = 0, fimDeJogo = 0;

    printf("Bem-vindo ao Jogo da Forca!\n");
    printf("Digite a palavra para comecar: ");
    scanf("%s", palavra);
    esconderPalavra(palavra, palavraOculta);

    while (!fimDeJogo) {
        desenharForca(erros);
        printf("Palavra: %s\n", palavraOculta);
        printf("Tentativas: %s\n", tentativas);
        printf("Digite uma letra ou 'q' para voltar ao menu: ");
        scanf(" %c", &tentativa);

        if (tentativa == 'q' || tentativa == 'Q') {
            printf("\nVoltando ao menu...\n");
            return;
        }

        // Verificar se a tentativa ja foi feita
        if (strchr(tentativas, tentativa)) {
            printf("Voce ja tentou essa letra. Tente outra.\n");
            continue;
        }

        // Adicionar tentativa ao historico
        tentativas[numTentativas] = tentativa;
        numTentativas++;

        // Verificar se a tentativa esta correta
        int acerto = 0;
        for (int i = 0; palavra[i] != '\0'; i++) {
            if (palavra[i] == tentativa) {
                palavraOculta[i] = tentativa;
                acerto = 1;
                acertos++;
            }
        }

        if (!acerto) {
            erros++;
            if (erros == MAX_TENTATIVAS) {
                fimDeJogo = 1;
                printf("\nVoce perdeu! A palavra era: %s\n", palavra);
            }
        } else if (venceu(palavraOculta)) {
            fimDeJogo = 1;
            printf("\nVoce venceu! A palavra era: %s\n", palavra);
        }
    }
}

void desenharForca(int erros) {
    printf("\n");
    printf("  _______\n");
    printf(" |       |\n");
    printf(" |       %c\n", (erros > 0) ? 'O' : ' ');
    printf(" |      %c%c%c\n", (erros > 2) ? '/' : ' ', (erros > 1) ? '|' : ' ', (erros > 3) ? '\\' : ' ');
    printf(" |       %c\n", (erros > 4) ? '/' : ' ');
    printf(" |      %c\n", (erros > 5) ? '\\' : ' ');
    printf(" |\n");
    printf(" |__\n");
}

void esconderPalavra(char* palavra, char* palavraOculta) {
    int i = 0;
    while (palavra[i] != '\0') {
        palavraOculta[i] = '_';
        i++;
    }
    palavraOculta[i] = '\0';
}

int venceu(char* palavraOculta) {
    for (int i = 0; palavraOculta[i] != '\0'; i++) {
        if (palavraOculta[i] == '_') return 0;
    }
    return 1;
}

// Funcao para detectar entrada do teclado sem bloquear
int kbhit(void) {
#ifdef _WIN32
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
#endif
}
