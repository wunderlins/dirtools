#!/usr/bin/env bash

h=`hostname`
p=`cat $h.pass`

if [[ "$h" == "wus-desktop4" ]]; then
	./ldap -D "cn=muana,ou=GenericMove,ou=Users,ou=USB,dc=ms,dc=uhbs,dc=ch" \
	       -b ou=USB,dc=ms,dc=uhbs,dc=ch \
	       -H ldap://ms.uhbs.ch \
	       -w "$p" \
	       "(&(objectClass=group)(cn=*B_M_Anaesthesiologie*))"
else
	./ldap -D "cn=admin,dc=intra,dc=wunderlin,dc=net" \
	       -b dc=intra,dc=wunderlin,dc=net \
	       -H ldap://dir.intra.wunderlin.net:389 \
	       -w "$p" \
	       "(objectClass=*)"
fi

