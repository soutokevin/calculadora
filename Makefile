all: build run

build:
	@gcc -Wall main.c -o main.out

shell:
	@./main.out --shell

run:
	@./main.out

mem: build
	valgrind ./main.out
