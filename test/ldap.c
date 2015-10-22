#include <ldap.h>
#include <stdio.h>
#include <string.h>
#include "ldap.h"

#define LDAP_SERVER "ldap://ms.uhbs.ch:389"
#define LDAP_USER "muana"
#define LDAP_PASS "anaana"
#define LDAP_BINDDN "cn=%s,ou=GenericMove,ou=Users,ou=USB,dc=ms,dc=uhbs,dc=ch"

LDAP  *ldap_connect(char* uri, char* binddn, char* bind_pw) {
	int   rc;
	LDAP *ld;
	
	//sprintf(bind_dn, LDAP_BINDDN, LDAP_USER);
	printf("Connecting as %s\n", binddn);
	
	if(ldap_initialize(&ld, uri)) {
		perror("ldap_initialize");
		return 0;
	}
	
	/*
	printf("ldap server: %s\n", uri);
	printf("ldap binddn: %s\n", binddn);
	printf("ldap bindpw: %s\n", bind_pw);
	*/
	
	rc = ldap_simple_bind_s(ld, binddn, bind_pw);
	if(rc != LDAP_SUCCESS) {
		fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(rc));
		return 0;
	}
	printf( "Successfully authenticated\n" );
	
	return ld;
}

int main(int argc, char** argv) {
	LDAP      *ld;
	int       rc;
	char      bind_dn[254];
	
	char url[100] = LDAP_SERVER;
	
	sprintf(bind_dn, LDAP_BINDDN, LDAP_USER);
	
 	ld = ldap_connect(url, bind_dn, LDAP_PASS);
	
	if (ld != NULL) {
		ldap_unbind_ext(ld, NULL, NULL);
		printf("Disconnected.\n");
	}
	
	return 0;
}

