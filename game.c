// ------ IMPORTS ------ //
#include <stdio.h>
#include <ncurses\ncurses.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

// =============================================================================
//                        NCURSES + CONFIGURACAO
// =============================================================================

// ------ INICIA AS FUNCIONALIDADES DO NCURSES ------
void iniciar_NCURSES() {
    initscr();              // Inicia a biblioteca ncurses
    cbreak();               // para receber inputs imediatamente
    noecho();               // não mostra números inseridos no terminal
    keypad(stdscr, TRUE);   // activa a leitura de teclas especiais
    curs_set(0);            // torna o cursor invisivel(0) no terminal; 1-normal; 2-pisca
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
        // detecta todos os cliques do rato + movimento + retira a necessidade de inserir valores no terminal
    start_color();          // cores no terminal
    setup_colors();         // paleta de cores escolhidas

}

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
}

// ------ CRIACAO E TAMANHO DAS JANELAS ------
void criar_janelas(JanelasJogo *wins) {
    wins->tabuleiro_win = newwin(19, 37, 1, 1);         // Tabuleiro
    wins->tempo_win = newwin(3, 20, 1, 40);             // Temporizador
    wins->botoes_win = newwin(5, 59, 21, 1);            // Botoes/comandos
    wins->num_terminados_win = newwin(15, 20, 5, 40);   // Numeros completos no tabuleiro

    keypad(wins->tabuleiro_win, TRUE);                  // Activa o teclado nno tabuleiro
}

// ------ TERMINA AS FUNCIONALIDADES DO NCURSES ------
void terminar_NCURSES() {
    endwin();   // Fecha a biblioteca ncurses
    return 0;
}

// =============================================================================
//                               JANELAS AUXILIARES
// =============================================================================
void mostrar_timer(JanelasJogo *wins) {
    moldura(wins->tempo_win, 0, 0);                     // Moldura básica
    mvwprintw(wins->tempo_win, 1, 2, "Tempo: 00:00");   // Texto a mostrar
    wrefresh(wins->tempo_win);                          // Mostra o timer
}

void mostrar_botoes(JanelasJogo *wins) {
    moldura(wins->botoes_win, 0, 0);
    mvwprintw(wins->botoes_win, 1, 2, "Setas/Rato: Movimento | Inserir: 1-9 | S: Sair");
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

    // Funcao para contar os numeros no tabuleiro
    contar_num(tabuleiro, contagem);

    // Preparar janela
    wclear(wins->num_terminados_win);                                       // Limpa janela
    moldura(wins->num_terminados_win, 0, 0);                                // Moldura básica
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

            // Celula seleccionada (4)
            if (i == state->select_y && j == state->select_x) {
                wattron(wins->tabuleiro_win, COLOR_PAIR(4));
            }

            // Cor dos numeros - SE CELULA NAO VAZIA
            if (state->tabuleiro[i][j].value != 0) {
                if (state->tabuleiro[i][j].is_error) {                     // Valor errado (3)
                    wattron(wins->tabuleiro_win, COLOR_PAIR(3));
                } else if (state->tabuleiro[i][j].is_fixed) {              // Valor fixo inicial (1)
                    wattron(wins->tabuleiro_win, COLOR_PAIR(1));
                } else {
                    wattron(wins->tabuleiro_win, COLOR_PAIR(2));    // Valor inserido pelo jogador (2)
                }

                // Insere o numero na celula desejada
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal, " %d ", state->tabuleiro[i][j].value);

                // Desligar cores do numero inserido/seleccionado
                wattroff(wins->tabuleiro_win, COLOR_PAIR(1));
                wattroff(wins->tabuleiro_win, COLOR_PAIR(2));
                wattroff(wins->tabuleiro_win, COLOR_PAIR(3));

            } else {
                // SE CELULA VAZIA - insere um ponto
                mvwprintw(wins->tabuleiro_win, esp_vertical, esp_horizontal, " . ");
            }

            // Desligar cores da celula seleccionada
            if (i == state->select_y && j == state->select_x) {
                wattroff(wins->tabuleiro_win, COLOR_PAIR(4));
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