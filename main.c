// ------ IMPORTS ------
#include <stdio.h>
#include <ncurses\ncurses.h> // Biblioteca ncurses, no windows está num subdirectório
#include <stdbool.h>          // Biblioteca booleana
#include "game.h"

int main(void) {

    int tabuleiro[SIZE][SIZE];
    iniciar_Tabuleiro(tabuleiro);
    desenhar_Tabuleiro(tabuleiro);

    // initscr(); // Inicializa a biblioteca ncurses



    // printf("Bem-vindo ao meu Jogo!\n");
    // printf("Estrutura inicial montada com sucesso.\n");


    // endwin();   // Finaliza a biblioteca ncurses
    return 0;
}