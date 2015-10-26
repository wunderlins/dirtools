.PHONY: ldap cmd

#CC = gcc

all: ldap cmd

ldap:
	$(CC) -o ldap -lldap ldap.c

cmd:
	$(CC) -o cmd -lreadline cmd.c
