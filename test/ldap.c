/*
	LDAP Search example
	Reference: http://www-archive.mozilla.org/directory/csdk-docs/search.htm
*/

#include <ldap.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "ldap.h"

#define LDAP_SERVER "ldap://ms.uhbs.ch:389"
#define LDAP_USER "muana"
#define LDAP_PASS "anaana"
#define LDAP_BINDDN "cn=%s,ou=GenericMove,ou=Users,ou=USB,dc=ms,dc=uhbs,dc=ch"
#define LDAP_SEARCH_BASE "ou=USB,dc=ms,dc=uhbs,dc=ch"
#define LDAP_GROUP_FILTER "(&(objectClass=group)(cn=*B_M_Anaesthesiologie*))"

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

char *guid2str(char *raw) {
	char str[40];
	sprintf(str, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
		raw[3], raw[2], raw[1], raw[0], raw[5], raw[4],
		raw[7], raw[6], raw[8], raw[9], raw[10], raw[11],
		raw[12], raw[13], raw[14], raw[15]
	);
	
	return str;
}

void ldap_display_attributes(LDAP *ld, LDAPMessage *msg) {
	// get all attributes
	char *a;
	BerElement *ber;
	
	for (a = ldap_first_attribute(ld, msg, &ber); 
		a != NULL; 
		a = ldap_next_attribute(ld, msg, ber)) {
		
		char **vals;
		int i;
		// Get the values of the attribute. 
		if ( ( vals = ldap_get_values( ld, msg, a ) ) != NULL ) {
			for ( i = 0; vals[i] != NULL; i++ ) {
				// Print the name of the attribute and each value
				if (strcmp(a, "objectGUID") == 0)
					printf("%s: %s\n", a, guid2str(vals[i]));
				else
					printf( "%s: %s\n", a, vals[i] );
			}
			// Free the attribute values from memory when done.
			ldap_value_free(vals);
		}
	}
	ldap_memfree(a);
	
	if ( ber != NULL ) {
		ber_free( ber, 0 );
	}
}

int main(int argc, char** argv) {
	LDAP      *ld;
	int       rc;
	char      bind_dn[254];
	LDAPMessage *res, *msg;
	int msgtype;
	char *dn;
	
	char url[254] = LDAP_SERVER;
	sprintf(bind_dn, LDAP_BINDDN, LDAP_USER);
	
 	ld = ldap_connect(url, bind_dn, LDAP_PASS);
	
	// fetch result
	rc = ldap_search_s(ld, LDAP_SEARCH_BASE, LDAP_SCOPE_SUBTREE, LDAP_GROUP_FILTER, NULL, 0, &res);
	if (rc != LDAP_SUCCESS) {
		if (ld != NULL) {
			ldap_unbind_ext(ld, NULL, NULL);
			printf("Disconnected.\n");
		}
		
		fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(rc));
		return 0;
	}
	
	// loop over result
	int num_entries = 0;
	int num_refs = 0;
	
	num_entries = ldap_count_entries( ld, res );
	num_refs = ldap_count_references( ld, res );
	printf("Number of entried: %d, number of References: %d\n", num_entries, num_refs);
	
	for ( msg = ldap_first_message( ld, res ); 
		msg != NULL; 
		msg = ldap_next_message( ld, msg ) ) {

		/* Determine what type of message was sent from the server. */
		msgtype = ldap_msgtype( msg );
		switch( msgtype ) {
			case LDAP_RES_SEARCH_ENTRY:
				// The result is an entry, print dn
				if ( ( dn = ldap_get_dn( ld, msg ) ) != NULL ) {
					printf( "==================================================\n");
					printf( "dn: %s\n", dn );
					/* Free the memory used for the DN when done */
					ldap_memfree( dn );
				}
				
				ldap_display_attributes(ld, msg);
				
				break;
			case LDAP_RES_SEARCH_REFERENCE:
				/* The result is a search reference. */
				break;
			case LDAP_RES_SEARCH_RESULT:
				/* The result is the final result sent by the server. */
				break;
		}
	}
	
	// disconnect
	if (ld != NULL) {
		ldap_unbind_ext(ld, NULL, NULL);
		printf("Disconnected.\n");
	}
	
	return 0;
}

