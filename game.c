// ------ IMPORTS ------ //
#include <stdio.h>
#include <ncurses\ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>         // para rand()
#include <time.h>           // para time(NULL)
#include "game.h"

// CONSTANTES NCURSES
#define LINHAS LINES
#define COLUNAS COLS

// =============================================================================
//                        NCURSES + CONFIGURACAO
// =============================================================================

// ------ CONFIG DAS CORES NO TABULEIRO ------
void setup_colors() {
    // 1. Numeros fixos (iniciais)
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    // 2. Numeros inseridos por jogador
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    // 3. Numeros errados
    init_pair(3, COLOR_RED, COLOR_BLACK);
    // 4. Célula seleccionada (inversos de 1)
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    // 5. Notas (numeros possiveis)
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    // 6. Numeros completos
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
    // 7. Numeros incompletos
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    // 8. Menu
    init_pair(8, COLOR_GREEN, COLOR_BLACK);
    // 9. Highlight linha / coluna
    init_pair(9, COLOR_WHITE, COLOR_BLUE); 
}

// ------ CONFIG DAS INSTRUÇÕES A APARECER NO MENU PRINCIPAL ------
void instrucoes_rodape() {
    // CONFIGURAR COR E CENTRAR AS INSTRUÇÕES
    attron(COLOR_PAIR(2) | A_ITALIC);
    const char* instrucoes = "Navegar: [^][v][<][>] + Rato | 1-9: Inserir | 0/Backspace: Apagar | ESC: Sair";
    mvprintw(LINHAS - 2, (COLUNAS - strlen(instrucoes))/2, "%s", instrucoes);
    attroff(COLOR_PAIR(2) | A_ITALIC);
}


// ------ INICIA AS FUNCIONALIDADES DO NCURSES ------
void iniciar_NCURSES() {
    initscr();              // Inicia a biblioteca ncurses
    cbreak();               // para receber inputs imediatamente
    noecho();               // não mostra números inseridos no terminal
    keypad(stdscr, TRUE);   // activa a leitura de teclas especiais
    curs_set(0);            // torna o cursor invisivel(0) no terminal; 1-normal; 2-pisca
    mousemask(ALL_MOUSE_EVENTS, NULL); // detecta todos os cliques do rato + movimento + retira a necessidade de inserir valores no terminal
    start_color();          // cores no terminal
    setup_colors();         // paleta de cores escolhidas

}


// ------ CRIACAO E TAMANHO DAS JANELAS ------
void criar_janelas(JanelasJogo *wins) {

    int largura_total = 37 + 22;
    int inicio_x = (COLUNAS - largura_total) / 2;
    int inicio_y = (LINHAS - 22) / 2;

    // (linhas, colunas, y, x)
    wins->tabuleiro_win = newwin(19, 37, inicio_y, inicio_x);               // Tabuleiro
    wins->tempo_win = newwin(3, 20, inicio_y, inicio_x + 40);               // Temporizador
    wins->botoes_win = newwin(15, 20, inicio_y + 3, inicio_x + 40);              // Botoes/comandos
    wins->num_terminados_win = newwin(3, 60, inicio_y + 19, inicio_x);      // Numeros completos no tabuleiro

    keypad(wins->tabuleiro_win, TRUE);                  // Activa o teclado nno tabuleiro
}

// ------ CRIAÇÃO DA MOLDURA DO TABULEIRO ------
void moldura(WINDOW *win, int x0, int y0) {
    box(win, 0, 0);
}

// ------ LER CLIQUES DO RATO ------
int ler_cliques(JanelasJogo *wins, int mouse_x, int mouse_y, int *sel_x, int *sel_y) {
    int win_x, win_y, altura, largura;

    getbegyx(wins->tabuleiro_win, win_y, win_x);
    getmaxyx(wins->tabuleiro_win, altura, largura);

    if (mouse_x >= win_x && mouse_x < win_x + largura &&
        mouse_y >= win_y && mouse_y < win_y + altura) {

        int local_x = mouse_x - win_x;
        int local_y = mouse_y - win_y;

        // Percorre todas as células e verifica qual foi clicada
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                int esp_vertical   = i * 2 + 1;
                int esp_horizontal = j * 4 + 1;

                // Clique dentro da área da célula (3 colunas de largura, 1 linha de altura)
                if (local_y == esp_vertical &&
                    local_x >= esp_horizontal && local_x < esp_horizontal + 3) {
                    *sel_x = j;
                    *sel_y = i;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// ------ TERMINA AS FUNCIONALIDADES DO NCURSES ------
void terminar_NCURSES() {
    endwin();   // Fecha a biblioteca ncurses
}

// TESTAR MOUSE - NAO FUNCIONA :(
void localizar_mouse(StatusJogo *state, JanelasJogo *wins, int teclas) {
    MEVENT event; 
    int ty, tx;
    getbegyx(wins->tabuleiro_win, ty, tx);

    if (teclas == KEY_MOUSE) {
        if (getmouse(&event) == OK) {
            // Escreve o debug dentro da janela do tempo para forçar visibilidade
            mvwprintw(wins->tempo_win, 2, 1, "X:%d Y:%d", event.x, event.y);
            wrefresh(wins->tempo_win);

            if (event.bstate & BUTTON1_CLICKED || event.bstate & BUTTON1_PRESSED) {
                ler_cliques(wins, event.x, event.y, &state->select_x, &state->select_y);
            }
        }
    }
}

// =============================================================================
//                               JANELAS AUXILIARES
// =============================================================================
void mostrar_timer(JanelasJogo *wins, StatusJogo *state) {
    time_t tempo_actual = time(NULL);      // Tempo atual
    double segundos_passados = difftime(tempo_actual, state->tempo_inicial);  // Diferenca entre o tempo atual e o tempo inicial

    int minutos = (int)segundos_passados / 60;   // Calcula os minutos
    int segundos = (int)segundos_passados % 60; // Calcula os segundos restantes

    wclear(wins->tempo_win);                             // Limpa janela de timer
    moldura(wins->tempo_win, 0, 0);                     // Moldura básica
    mvwprintw(wins->tempo_win, 1, 2, "Tempo:  %02d:%02d", minutos, segundos);
    wrefresh(wins->tempo_win);                          // Mostra o timer
}

void mostrar_botoes(JanelasJogo *wins, int dificuldade) {

    const char *nivel;
    if (dificuldade == 40) nivel = "Facil";
    else if (dificuldade == 50) nivel = "Medio";
    else nivel = "Dificil";

    wclear(wins->botoes_win);
    moldura(wins->botoes_win, 0, 0);
    mvwprintw(wins->botoes_win, 1, 2, "   Dif: %s", nivel);
    wrefresh(wins->botoes_win);
}

void contar_num(Celula tabuleiro[SIZE][SIZE], int contar[10]) {
    
    for (int c = 0; c < 10; c++) {      // Ciclo para contar o numero de vezes que cada numero aparece
        contar[c] = 0;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int num = tabuleiro[i][j].value;       // Guarda o valor na celula
            
            if (num > 0 && !tabuleiro[i][j].is_error){    // Se > 0 e está certo, adiciona ao array
                contar[num] ++;
            }
        }
    }
}

void lista_num(JanelasJogo *wins, int contar[10]) {

    for (int n = 1; n <= 9; n++) {

        // Posicao dos numeros - 2 + espaço entre numeros
        int posicao = 2 + (n - 1) * 3;

        if (contar[n] == 9) {    // Se já existem 9 numeros iguais, muda a cor de fundo para verde
            wattron(wins->num_terminados_win, COLOR_PAIR(6) | A_BOLD);
            mvwprintw(wins->num_terminados_win, 1, posicao, " %d ", n);
            wattroff(wins->num_terminados_win, COLOR_PAIR(6) | A_BOLD);
        } else {
            // numeros incompletos, cor do fundo vermelho
            wattron(wins->num_terminados_win, COLOR_PAIR(7));
            mvwprintw(wins->num_terminados_win, 1, posicao, " %d ", n);
            wattroff(wins->num_terminados_win, COLOR_PAIR(7));
        }
    }
}

void mostrar_nums_concl(JanelasJogo *wins, Celula tabuleiro[SIZE][SIZE]) {
    int contagem[10];

    wclear(wins->num_terminados_win);                   // Limpa janela de numeros concluidos

    // Funcao para contar os numeros no tabuleiro
    contar_num(tabuleiro, contagem);

    // Preparar janela
    box(wins->num_terminados_win, 0, 0);                                    // Moldura básica
    mvwprintw(wins->num_terminados_win, 0, 2, " Numeros Concluidos: ");     // Texto a mostrar 

    // Funcao para a lista de numeros concluidos
    lista_num(wins, contagem);

    // Mostra os numeros concluidos
    wrefresh(wins->num_terminados_win);
}




// =============================================================================
//                               JANELA TABULEIRO
// =============================================================================
void iniciar_Tabuleiro(Celula tabuleiro[SIZE][SIZE]) { // Testar
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            tabuleiro[i][j].value = 0;          // Inicia o tabuleiro com 0 - celula vazia
            tabuleiro[i][j].is_fixed = false;   // Célula vazia
            tabuleiro[i][j].is_error = false;   // Valor inserido correcto
        }
    }
}


// ------ CRIAR TABULEIRO (NA JANELA) ------
void desenhar_Tabuleiro(JanelasJogo *wins, StatusJogo *state) {
    wclear(wins->tabuleiro_win);            // Limpa tabuleiro
    moldura(wins->tabuleiro_win, 0, 0);     // Desenha a moldura

    // Desenho do tabuleiro
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int esp_vertical = i * 2 + 1;    // Espaçamento vertical - 1 linha vazia (1, 3, 5, ...)
            int esp_horizontal = j * 4 + 1;  // Espacamento horizontal - 3 colunas vazias (2, 6, 10, ...)

            // Linha e coluna da célula seleccionada
            bool seleccionada  = (i == state->select_y && j == state->select_x);
            bool na_cruz       = (i == state->select_y || j == state->select_x) && !seleccionada;

            if (seleccionada && state->tabuleiro[i][j].value != 0) {
                // Célula seleccionada COM número — combina fundo azul com cor do número
                if (state->tabuleiro[i][j].is_error) {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(3) | A_REVERSE);
                } else if (state->tabuleiro[i][j].is_fixed) {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(4));   // fundo cyan, numero fixo
                } else {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(4));   // fundo cyan, numero jogador
                }
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal,
                        " %d ", state->tabuleiro[i][j].value);
                wattroff(wins->tabuleiro_win, COLOR_PAIR(3) | COLOR_PAIR(4) | A_REVERSE);

            } else if (na_cruz && state->tabuleiro[i][j].value != 0) {
                if (state->tabuleiro[i][j].is_error) {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(3) | A_REVERSE);
                } else {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(9));
                }
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal,
                        " %d ", state->tabuleiro[i][j].value);
                wattroff(wins->tabuleiro_win, COLOR_PAIR(3) | COLOR_PAIR(9) | A_REVERSE);

            } else if (na_cruz) {
                wattron(wins->tabuleiro_win, COLOR_PAIR(9));
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal, " . ");
                wattroff(wins->tabuleiro_win, COLOR_PAIR(9));

            } else if (seleccionada) {
                // Célula seleccionada VAZIA
                wattron(wins->tabuleiro_win, COLOR_PAIR(4));
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal, " . ");
                wattroff(wins->tabuleiro_win, COLOR_PAIR(4));

            } else if (state->tabuleiro[i][j].value != 0) {
                // Célula normal COM número
                if (state->tabuleiro[i][j].is_error) {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(3));
                } else if (state->tabuleiro[i][j].is_fixed) {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(1));
                } else {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(2));
                }
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal,
                        " %d ", state->tabuleiro[i][j].value);
                wattroff(wins->tabuleiro_win, COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));

            } else {
                // Célula normal VAZIA
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal, " . ");
            }
        }

        // Desenhar linhas grossas (3x3)
        for (int i = 1; i < 3; i++) {
            mvwhline(wins->tabuleiro_win, i * 6, 1, ACS_HLINE, 35);     // Linha horizontal contínua
            mvwvline(wins->tabuleiro_win, 1, i * 12, ACS_VLINE, 17);    // Linha vertical contínua
        }

        // Mostrar o tabuleiro
        wrefresh(wins->tabuleiro_win);
    }
}


// =============================================================================
//                            LOGICA SUDOKU
// =============================================================================
int verificarJogada(int tabuleiro[SIZE][SIZE], int linha, int coluna, int value) {

    // Verificar se o numero ja existe na linha
    for (int x = 0; x < SIZE; x++) {
        if (tabuleiro[linha][x] == value) {
            return 0;
        }
    }

    // Verificar se o numero ja existe na coluna
    for (int y = 0; y < SIZE; y++) {
        if (tabuleiro[y][coluna] == value) {
            return 0;
        }
    }

    // Verificar se já existe no quadrado 3x3
    int quad_x = linha - (linha % 3);
    int quad_y = coluna - (coluna % 3);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[quad_x + i][quad_y + j] == value) {
                return 0;
            }
        }
    }

    return 1; // Jogada válida
}



// =============================================================================
//                  PREENCHER TABULEIRO COM NUMEROS INICIAIS
// =============================================================================
bool encontrarCelulaVazia(Celula tabuleiro[SIZE][SIZE], int *linha, int *coluna) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (tabuleiro[i][j].value == 0) {
                *linha = i;
                *coluna = j;
                return true;
            }
        }
    }
    return false;   
}


void baralharNumeros(int *numeros, int n) {      // Para não serem em ordem no tabuleiro inicial

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }
}

bool preencherTabuleiro(Celula tabuleiro[SIZE][SIZE]) {
    int linha, coluna;

    // Se não existirem células vazias -> tabuleiro completo ->OK
    if (!encontrarCelulaVazia(tabuleiro, &linha, &coluna)) {
        return true;
    }

    // Preencher celulas vazias com numeros aleatórios
    int numeros[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    baralharNumeros(numeros, 9);

    // Criar uma matriz auxiliar para preencher o tabuleiro
    int matriz_aux[SIZE][SIZE];
    for (int a = 0; a < SIZE; a++) {
        for (int b = 0; b < SIZE; b++) {
            matriz_aux[a][b] = tabuleiro[a][b].value;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        int num = numeros[i];

        // Verificar se o numero pode ser inserido na celula
        if (verificarJogada(matriz_aux, linha, coluna, num)) {
            tabuleiro[linha][coluna].value = num;
            tabuleiro[linha][coluna].is_fixed = true; // Assumir que o numero foi inserido certo
            
            matriz_aux[linha][coluna] = num;          // Actualizar a matriz auxiliar

            if (preencherTabuleiro(tabuleiro)) {      // Tenta preencher a proxima celula - recursividade
                return true;
            }

            tabuleiro[linha][coluna].value = 0;     // Se falhar, volta ao estado anterior
            tabuleiro[linha][coluna].is_fixed = false;
            matriz_aux[linha][coluna] = 0;
        }
    }
    return false; // Recua para a cécula anterior
}


void novoJogo(Celula tabuleiro[SIZE][SIZE], int dificuldade) {

    srand(time(NULL));      // Inicializar o gerador de numeros aleatorios

    iniciar_Tabuleiro(tabuleiro);   // Limpa o tabuleiro
    preencherTabuleiro(tabuleiro);  // Preenche o tabuleiro inicial completamente

    // retirar numeros do tabuleiro, dependendo da dificuldade (numeros visiveis no tabuleiro inicial)
    int removidos = 0;

    while (removidos < (81 - dificuldade)) {

        int l = rand() % SIZE;
        int c = rand() % SIZE;

        if (tabuleiro[l][c].value != 0) {
            tabuleiro[l][c].value = 0;
            tabuleiro[l][c].is_fixed = false;
            removidos++;
        }
    }
}


// =============================================================================
//                            FUNÇÃO AUXILIAR AO MAIN
// =============================================================================
void jogar_numero(StatusJogo *state, int num) {
    int y = state->select_y;
    int x = state->select_x;

    // Só permite jogar se não for uma célula fixa (inicial)
    if (state->tabuleiro[y][x].is_fixed) return;

    // Se o número for 0, limpa a célula
    if (num == 0) {
        state->tabuleiro[y][x].value = 0;
        state->tabuleiro[y][x].is_error = false;
        return;
    }

    // Criar matriz auxiliar de ints UMA vez para validar
    int aux[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            aux[i][j] = state->tabuleiro[i][j].value;
        }
    }

    // Retira o valor atual da célula na matriz auxiliar para validar o novo
    aux[y][x] = 0;

    state->tabuleiro[y][x].value = num;
    
    // Verifica se a jogada é válida segundo as regras do Sudoku
    if (verificarJogada(aux, y, x, num)) {
        state->tabuleiro[y][x].is_error = false;
    } else {
        state->tabuleiro[y][x].is_error = true;
    }
}


// =============================================================================
//                            VITORIA
// =============================================================================
bool verificar_vitoria(Celula tabuleiro[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (tabuleiro[i][j].value == 0 || tabuleiro[i][j].is_error)
                return false;
        }
    }
    return true;
}

void mostrar_vitoria(JanelasJogo *wins, StatusJogo *state) {
    const char *texto[] = {
        "                                                                              ",
        "   _|      _|                        _|          _|  _|                _|    ",
        "     _|  _|    _|_|    _|    _|      _|          _|      _|_|_|        _|      ",
        "       _|    _|    _|  _|    _|      _|    _|    _|  _|  _|    _|      _|     ",
        "       _|    _|    _|  _|    _|        _|  _|  _|    _|  _|    _|            ",
        "       _|      _|_|      _|_|_|          _|  _|      _|  _|    _|      _|     ",
        "                                                                              "
    };

    int num_linhas = 7;

    clear();
    attron(COLOR_PAIR(8) | A_BOLD);
    for (int i = 0; i < num_linhas; i++) {
        int x = (COLUNAS - (int)strlen(texto[i])) / 2;
        if (x < 0) x = 0;
        mvprintw(LINHAS / 2 - num_linhas / 2 + i, x, "%s", texto[i]);
    }
    attroff(COLOR_PAIR(8) | A_BOLD);

    attron(COLOR_PAIR(2));
    mvprintw(LINHAS / 2 + num_linhas / 2 + 2, (COLUNAS - 30) / 2, "Carrega numa tecla para continuar...");
    attroff(COLOR_PAIR(2));

    refresh();
    nodelay(stdscr, FALSE);   // espera por uma tecla (bloqueia)
    getch();
    state->running = false;
}