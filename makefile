#!/usr/bin/make -f
microshell: main.c
	gcc -Wall -ansi -o microshell main.c
	./microshell
	rm microshell

