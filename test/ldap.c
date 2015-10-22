#include <ldap.h>
#include <stdio.h>
#include <string.h>

#define LDAP_SERVER "ldap://ms.uhbs.ch:389"
#define LDAP_USER "muana"
#define LDAP_PASS "anaana"
#define LDAP_BINDDN "cn=%s,ou=GenericMove,ou=Users,ou=USB,dc=ms,dc=uhbs,dc=ch"

int main(int argc, char** argv) {
	LDAP      *ld;
	int       rc;
	char      bind_dn[100];
	struct    berval creds;
	creds.bv_val = LDAP_PASS;
	
	sprintf(bind_dn, LDAP_BINDDN, LDAP_USER);
	printf( "Connecting as %s...\n", bind_dn );
 
	/* Open LDAP Connection */
	if(ldap_initialize( &ld, LDAP_SERVER)) {
		perror("ldap_initialize");
		return(1);
	}
	
	/* User authentication (bind) */
	rc = ldap_simple_bind_s(ld, bind_dn, LDAP_PASS);
	//rc = ldap_sasl_bind_s(ld, bind_dn, LDAP_SASL_SIMPLE , &creds, NULL, NULL, NULL); //returns ldap error code 49 but continues working
	if(rc != LDAP_SUCCESS) {
		fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(rc) );
		return(1);
	}
	
	printf( "Successful authentication\n" );
	ldap_unbind_ext(ld, NULL, NULL);
	return( 0 );
}
