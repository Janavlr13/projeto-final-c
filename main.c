#include <stdio.h>
#include <ncurses\ncurses.h>

int main(void) {
    initscr(); // Inicializa a biblioteca ncurses



    printf("Bem-vindo ao meu Jogo!\n");
    printf("Estrutura inicial montada com sucesso.\n");


    endwin();   // Finaliza a biblioteca ncurses
    return 0;
}