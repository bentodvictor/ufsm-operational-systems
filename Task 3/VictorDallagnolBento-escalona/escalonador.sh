#!/bin/sh
chmod +x escalonador.sh
clear
gcc -o escalonador main.c arrive.c confere_CPU.c CriarProcessos.c fluxos.c inputs.c insere_CPU.c organizaFilas.c ready.c io.c insere_IO.c confere_IO.c
./escalonador
