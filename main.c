// ------ IMPORTS ------
#include <stdio.h>
#include <ncurses\ncurses.h> // Biblioteca ncurses, no windows está num subdirectório
#include <stdbool.h>         // Biblioteca booleana
#include <string.h>          // para strings
#include <stdlib.h>          // para rand()
#include <time.h>            // para time(NULL)
#include "game.h"


// CONSTANTES NCURSES
#define LINHAS LINES
#define COLUNAS COLS

// =============================================================================
//                               MENU PRINCIPAL
// =============================================================================
int menu_principal() {
    clear();    // Limpa a tela

    const char* titulo[] = {   // https://patorjk.com/software/taag/#p=display&f=Block&t=Sudoku&x=none&v=4&h=4&w=80&we=false
        "_|_|_|                  _|            _|                 ",
        "_|      _|    _|    _|_|_|    _|_|    _|  _|    _|    _| ",
        "_|_|    _|    _|  _|    _|  _|    _|  _|_|      _|    _| ",
        "    _|  _|    _|  _|    _|  _|    _|  _|  _|    _|    _| ",
        "_|_|_|    _|_|_|    _|_|_|    _|_|    _|    _|    _|_|_| "
    };

    // CONFIGURAR COR E CENTRAR O TITULO
    int linhas_titulo = 5;
    attron(COLOR_PAIR(1) | A_BOLD);
    for (int i = 0; i < linhas_titulo; i++) {
        int largura_titulo = (int)strlen(titulo[i]);
        mvprintw(LINHAS/2 - 10 + i, (COLUNAS - largura_titulo)/2, "%s", titulo[i]);
    }
    attroff(COLOR_PAIR(1) | A_BOLD);


    const char* opcoes[] = {
        "[1] Novo Jogo - Facil",
        "[2] Novo Jogo - Medio",
        "[3] Novo Jogo - Dificil",
        "[0] Sair"
    };

    // CONFIGURAR COR E CENTRAR O MENU
    attron(COLOR_PAIR(8));
    for (int i = 0; i < 4; i++) {
        mvprintw(LINHAS/2 + i, (COLUNAS - 25)/2, "%s", opcoes[i]);
    }
    attroff(COLOR_PAIR(8));

    instrucoes_rodape();

    refresh();      // Actualiza a tela
    return getch();
}


// =============================================================================
//                                  FUNÇÃO MAIN
// =============================================================================
int main() {

    iniciar_NCURSES();

    // Verificar se o terminal tem pelo menos 30 linhas e 80 colunas
    if (LINES < 30 || COLS < 80) {
    terminar_NCURSES();
    printf("Erro: Aumenta o tamanho da janela do terminal antes de jogar!\n");
    return 1;
    }

    int teclas = menu_principal();
    int dificuldade = 40;           // Dificuldade padrão - fácil
    
    if (teclas == '1') {
        dificuldade = 40;
    } else if (teclas == '2') {
        dificuldade = 50;
    } else if (teclas == '3') {
        dificuldade = 60;
    } else if (teclas == '0' || teclas == 27) {         // 27 =ESC
        terminar_NCURSES();
        return 0;
    }

    // Limpa o menu para não aparecer debaixo do jogo
    clear();
    refresh();

    StatusJogo state;               // Estado do jogo
    JanelasJogo wins;               // Janelas do jogo

    state.running = true;
    state.select_x = 0;
    state.select_y = 0;
    state.dificuldade = dificuldade;

    criar_janelas(&wins);                     // Cria as janelas do jogo
    novoJogo(state.tabuleiro, dificuldade);   // Inicia um novo jogo, pela dificuldade escolhida
    state.tempo_inicial = time(NULL);         // Inicia o temporizador

    MEVENT event;                                            // DETECTA CLIQUES DO RATO
    
    while (state.running) {
        erase();                                           // Limpa a tela
        instrucoes_rodape();                                 // Desenha as instrucoes no rodape

        refresh();                                           // Actualiza a tela

        desenhar_Tabuleiro(&wins, &state);                   // Desenha o tabuleiro na janela
        mostrar_botoes(&wins, dificuldade);                               // Desenha os botoes na janela
        mostrar_timer(&wins, &state);                        // Desenha o temporizador na janela
        mostrar_nums_concl(&wins, state.tabuleiro);          // Desenha os numeros concluidos na janela
        timeout(500);

        int teclas = getch();                                // Leitura das teclas pressionadas

        switch (teclas) {
            // TECLADO SETAS
            case KEY_UP:
                if (state.select_y > 0) state.select_y--;
                break;
            case KEY_DOWN:
                if (state.select_y < SIZE - 1) state.select_y++;
                break;
            case KEY_LEFT:
                if (state.select_x > 0) state.select_x--;
                break;
            case KEY_RIGHT:
                if (state.select_x < SIZE - 1) state.select_x++;
                break;

            // RATO
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    if (event.bstate & (BUTTON1_PRESSED | BUTTON1_RELEASED | 
                            BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED)) {
                        ler_cliques(&wins, event.x, event.y, &state.select_x, &state.select_y);
                    }
                }
                break;
            
            case 27:                                            // Sair do jogo
                state.running = false;
                break;

            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':    // Insere o numero na celula desejada
            {
            jogar_numero(&state, teclas - '0');
             if (verificar_vitoria(state.tabuleiro)) {
                mostrar_vitoria(&wins, &state);
            }
            break;
            }

            case '0': case KEY_BACKSPACE: case 127:
                jogar_numero(&state, 0);
                break;
        }
    }

    terminar_NCURSES();
    return 0;
}