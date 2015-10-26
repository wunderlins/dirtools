#!/usr/bin/env bash

ldapsearch -b dc=intra,dc=wunderlin,dc=net -s sub -D cn=admin,dc=intra,dc=wunderlin,dc=net -H ldap://dir:389 -W -x
