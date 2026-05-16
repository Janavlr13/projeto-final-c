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
    time_t tempo_inicial;               // Tempo de inicio
    int dificuldade;                    // Dificuldade
} StatusJogo;


// =============================================================================
//                          PROTOTIPOS DAS FUNCOES
// =============================================================================

// ------ FUNCOES NCURSES ------
void iniciar_NCURSES();
void criar_janelas(JanelasJogo *wins);
void terminar_NCURSES();

// ------ FUNCOES LOGICA + JOGO ------
void novoJogo(Celula tabuleiro[SIZE][SIZE], int dificuldade);
int verificarJogada(int tabuleiro[SIZE][SIZE], int linha, int coluna, int value);
int ler_cliques(JanelasJogo *wins, int mouse_x, int mouse_y, int *sel_x, int *sel_y);
void jogar_numero(StatusJogo *state, int num);
bool verificar_vitoria(Celula tabuleiro[SIZE][SIZE]);

// ------ FUNCOES INTERFACE ------
void desenhar_Tabuleiro(JanelasJogo *wins, StatusJogo *state);
void mostrar_timer(JanelasJogo *wins, StatusJogo *state);
void mostrar_botoes(JanelasJogo *wins, int dificuldade);
void mostrar_nums_concl(JanelasJogo *wins, Celula tabuleiro[SIZE][SIZE]);
void instrucoes_rodape();
void localizar_mouse(StatusJogo *state, JanelasJogo *wins, int teclas);
void mostrar_vitoria(JanelasJogo *wins, StatusJogo *state);

#endif