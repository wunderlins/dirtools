#!/usr/bin/env bash

uri="ldap://dir"
basedn="dc=intra,dc=wunderlin,dc=net"
admin="admin"
filter="(objectClass=*)"
pass="e33k44"

ldapsearch -H "$uri" \
           -D "cn=$admin,$basedn" \
           -w "$pass" \
           -b "$basedn" \
           -S sub \
           -x "$filter"

