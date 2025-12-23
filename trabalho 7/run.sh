#!/bin/bash
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
