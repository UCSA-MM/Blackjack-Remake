all : main server

main : main.c gamewindow.c cards.c gamemenu.c
	gcc -Wall -Wextra -pedantic --verbose -O2 -g \
	main.c cards.c gamewindow.c gamemenu.c \
	-o main \
	-I ../include -L ../lib -lraylib -lm

server : server.c
	gcc -Wall -Wextra -pedantic --verbose -O2 -g \
	server.c \
	-o server \
	-lsqlite3

.PHONY: all
