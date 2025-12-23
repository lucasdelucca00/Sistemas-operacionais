#!/bin/bash
# Limpa pipes antigos se existirem travados
rm -f /tmp/fifo_javalis /tmp/fifo_garcom

./cozinheiro &
sleep 1 
./gaules L &
./gaules U &
./gaules C &
./gaules A &
./gaules S &

read -p "Pressione Enter para encerrar..."
pkill cozinheiro
pkill gaules
# Limpeza final
rm -f /tmp/fifo_javalis /tmp/fifo_garcom
