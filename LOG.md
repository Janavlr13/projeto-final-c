==========================================================================
Sessão 1 – 28/04/2026
==========================================================================
Funcionalidades implementadas:
 - Decidi fazer um jogo de Sudoku: vou começar por normal(9x9), 4x4, 12x12
 - Criação da pasta Actividade6
 - Criação do repositório no GitHub: projeto-final-c
 - Configuração do VSCode para utilizar commits para o repositório
 - Criação dos ficheiros da actividade: main.c, game.c, game.h, LOG.md, README.md

Maior dificuldade:
 - Utilizar comandos no terminal do VSCode. Prefiro que seja automático.

Como resolvi (ou não resolvi):
 - Não resolvi.

Próximo passo planeado:
 - Começar a pensar no código e o que quero incluir no projecto, se quero variantes ou faço só o sudoku normal.

Linhas de código escritas (estimativa):
 - 0


==========================================================================
Sessão 2 – 29/04/2026
==========================================================================
Funcionalidades implementadas:
 - Tentativa de implementação da biblioteca ncurses para a interface visual.

Maior dificuldade:
 - Erros de linker (undefined reference to '__imp_initscr'; undefined reference to `__imp_endwin') ao compilar no windows.

Como resolvi (ou não resolvi):
 - Usar um subdirectório: #include <ncurses\ncurses.h>
 - Usar o comando: gcc main.c game.c -o jogo -DNCURSES_STATIC -I/msys64/ucrt64/include/ncursesw -L/msys64/ucrt64/lib -lncursesw

Próximo passo planeado:
 - Escrever código da versão mais básica do sudoku.
 - Tentar usar as funcionalidades do ncurses para verificar o seu funcionamento.

Linhas de código escritas (estimativa):
 - aprox. 15