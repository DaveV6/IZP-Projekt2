CFLAGS=-std=c11 -g -Wall -Wextra -Werror

maze: maze.c
	gcc $(CFLAGS) maze.c -o maze