cmake_minimum_required(VERSION 3.23)
project(BetriebsystemeLabor)

set(CMAKE_CXX_STANDARD 23)

add_executable(BetriebsystemeLabor Aufgabe0/nr4/printString.c Aufgabe1/nChildprocess.c Aufgabe1/simple-fork.c Aufgabe1/fork-example.c Aufgabe3/unnamedPipes.c Aufgabe3/fifo_send.c Aufgabe3/fifo_recv.c)
