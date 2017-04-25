all: build run

build:
	@gcc -Wall main.c -o main.out

run:
	@./main.out

mem: build
	valgrind ./main.out
