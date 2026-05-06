#ifndef game_h
#define game_h

// ----- IMPORTS -----
#include <ncurses\ncurses.h>
#include <stdbool.h>

// ------ TAMANHO DO TABULEIRO ------
#define SIZE 9     // 9x9

// ------ CONFIG CELULAS DO TABULEIRO ------
typedef struct {
    int value;
    bool is_fixed;      // True - valor fixo inicial; False - célula vazia
    bool notes[10];     // Valor de 1 a 9 como nota (valor possível)
    bool is_error;      // True - valor inserido errado; False - valor inserido certo
} Celula;

// ------ CONFIG JANELAS ------
typedef struct {
    WINDOW *tabuleiro_win;              // Tabuleiro
    WINDOW *tempo_win;                  // Temporizador
    WINDOW *botoes_win;                 // Botoes/comandos
    WINDOW *num_terminados_win;         // Numeros completos no tabuleiro
} JanelasJogo;

// ------ CONFIG ESTADO DO JOGO ------
typedef struct {
    Celula tabuleiro[SIZE][SIZE];       // Tamanho das celulas do tabuleiro
    int select_x, select_y;             // Celula seleccionada
    bool running;                       // Jogo a correr (True) ou parado (False)
} StatusJogo;



// ----- PROTOTIPOS DAS FUNCOES -----
void iniciar_NCURSES();
void setup_colors();
void criar_janelas(JanelasJogo *wins);
void terminar_NCURSES();

void mostrar_timer(JanelasJogo *wins);
void mostrar_botoes(JanelasJogo *wins);
void contar_num(Celula tabuleiro[SIZE][SIZE], int contar[10]);
void lista_num(JanelasJogo *wins, int contar[10]);
void mostrar_nums_concl(JanelasJogo *wins, Celula tabuleiro[SIZE][SIZE]);


void iniciar_Tabuleiro(Celula tabuleiro[SIZE][SIZE]);
void desenhar_Tabuleiro(JanelasJogo *wins, StatusJogo *state);




#endif