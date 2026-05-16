@echo off
title Compilador Sudoku
color 0B

echo =========================================
echo    COMPILANDO SUDOKU EM C...
echo =========================================

:: 1. Compila o código (ajusta os nomes se necessário)
gcc main.c game.c -o jogo -DNCURSES_STATIC -I/msys64/ucrt64/include/ncursesw -L/msys64/ucrt64/lib -lncursesw


:: 2. Verifica se a compilação teve sucesso
if %errorlevel% neq 0 (
    color 0C
    echo.
    echo [ERRO] A compilacao falhou! Verifica os erros acima.
    pause
    exit /b
)

echo [OK] Compilacao concluida com sucesso!
echo.
echo A abrir o jogo numa nova janela...
echo.

:: 3. Abre o jogo numa nova janela de CMD separada e fecha esta
start "Sudoku Game" cmd /c "jogo.exe & pause"
exit