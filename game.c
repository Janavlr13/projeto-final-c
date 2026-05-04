// ------ IMPORTS ------ //
#include <stdio.h>
#include <ncurses\ncurses.h>
#include <stdbool.h>
#include "game.h"

// ------------------------------ NCURSES ----------------------------- //
void iniciar_NCURSES() {
    initscr(); // Inicia a biblioteca ncurses
}


void terminar_NCURSES() {
    endwin();   // Fecha a biblioteca ncurses
}

// ------------------------------ TABULEIRO ----------------------------- //
void iniciar_Tabuleiro(int tabuleiro[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            tabuleiro[i][j] = 0;   // Inicia o tabuleiro com 0 - celula vazia
        }
    }
}


void desenhar_Tabuleiro(int tabuleiro[SIZE][SIZE]) {
    printf("-------------------------\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (j%3 == 0) {
                printf("| ");
            }
            if (tabuleiro[i][j] == 0) {
                printf(". "); // Celula vazia
            } else {
                printf("%d ", tabuleiro[i][j]);
            }
        }
        printf("|\n");
        if ((i + 1) % 3 == 0) {
            printf("-------------------------\n");
        }
    }
    
}

// -------------------------- LOGICA SUDOKU ---------------------------- //

