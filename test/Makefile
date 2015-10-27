.PHONY: ldap cmd 

CC=gcc
CFLAGS=
LIBS=-lldap -lreadline

#CC = gcc
HOSTNAME = $(shell hostname)

all: ldap cmd

hostname:
	@echo Hostname: $(HOSTNAME)
	
ldap: hostname
	$(CC) -D $(HOSTNAME) -o ldap $(LIBS) $(CFLAGS) ldap.c

cmd:
	$(CC) -D $(HOSTNAME) -o cmd $(LIBS) $(CFLAGS) cmd.c

