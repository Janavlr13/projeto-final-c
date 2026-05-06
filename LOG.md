
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



Sessão 2 – 29/04/2026
==========================================================================
Funcionalidades implementadas:
 - Tentativa de implementação da biblioteca ncurses para a interface visual.

Maior dificuldade:
 - Erros de linker (undefined reference to '__imp_initscr'; undefined reference to `__imp_endwin') ao compilar no windows.

Como resolvi (ou não resolvi):
 - Usar um subdirectório: #include <ncurses\ncurses.h>, devido a ter o W11.
 - Usar o comando: gcc main.c game.c -o jogo -DNCURSES_STATIC -I/msys64/ucrt64/include/ncursesw -L/msys64/ucrt64/lib -lncursesw

Próximo passo planeado:
 - Escrever código da versão mais básica do sudoku.
 - Tentar usar as funcionalidades do ncurses para verificar o seu funcionamento.

Linhas de código escritas (estimativa):
 - aprox. 15


Sessão 3 – 04/05/2026
==========================================================================
Funcionalidades implementadas:
 - Escolha das funcionalidades do ncurses a utilizar no projecto:
    - Cores;
    - Detecção do Clique do rato, em vez de escrever linha e coluna;
    - Criação de notas (numeros possiveis) em cada célula do tabuleiro;
    - Utilização de Múltiplas Janelas;
    - Avisos sonoros quando o numero inserido está errado;
    - Atributos nas céculas seleccionadas.
 - Criação do tabuleiro básico.
 - Organização e hierarquia dos ficheiros do projecto.

Maior dificuldade:
 - Perceber as capacidades do ncurses e como as mesmas funcionam e podem ser implementadas 

Como resolvi (ou não resolvi):
 - Ainda não resolvi. Vou ver alguns videos a explicar o assunto.
 - https://terminalroot.com.br/ncurses/

Próximo passo planeado:
 - Continuar a estudar o ncurses
 - Criar a logica do sudoku

Linhas de código escritas (estimativa):
 - aprox. 50


 Sessão 4 – 06/05/2026
==========================================================================
Funcionalidades implementadas:
 - Criação de typedef struct - aliases para as configurações de Celulas, Janelas, e Jogo;
 - Edição do game.h - adicionei as aliases e os protótipos das funções existentes no game.c;
 - Em game.c:
   - Criação de funções para a utilização e configuração do ncurses;
   - Criação de funções para mostrar janelas auxiliares básicas para o jogo - relógio, botões e numeros concluídos;
   - Criação de uma janela tabuleiro;
 - Em todas as funções utilizei funcionalidades do ncurses.

Maior dificuldade:
 - Utilizar funcionalidades do ncurses.

Como resolvi (ou não resolvi):
 - Com a prática torna-se mais fácil mudar a forma de raciocinio para a inclusão do ncurses.

Próximo passo planeado:
 - Continuar a configurar o jogo;
 - Criar o código de raciocínio do jogo do sudoku.

Linhas de código escritas (estimativa):
 - aproxi. 250