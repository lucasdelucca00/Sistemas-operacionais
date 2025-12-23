#!/bin/bash
./cozinheiro &
sleep 1 # Garante que o cozinheiro criou a memória
./gaules L &
./gaules U &
./gaules C &
./gaules A &
./gaules S &

# Aguarda input para matar tudo
read -p "Pressione Enter para encerrar..."
pkill cozinheiro
pkill gaules
