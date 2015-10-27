.PHONY: realpath test

CC=gcc
CFLAGS=
LIBS=-lldap -lreadline

#CC = gcc
HOSTNAME = $(shell hostname)

all: realpath test

test:
	$(MAKE) -C test

realpath:
	gcc -D $(HOSTNAME) -o bin/realpath $(LIBS) $(CFLAGS) lib/realpath.c
