/*
	LDAP Search example
	Reference: http://www-archive.mozilla.org/directory/csdk-docs/search.htm
*/

#include <ldap.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "ldap.h"

#define LDAP_SERVER "ldap://ms.uhbs.ch:389"
#define LDAP_USER "muana"
#define LDAP_PASS "anaana"
#define LDAP_BINDDN "cn=%s,ou=GenericMove,ou=Users,ou=USB,dc=ms,dc=uhbs,dc=ch"
#define LDAP_SEARCH_BASE "ou=USB,dc=ms,dc=uhbs,dc=ch"
#define LDAP_GROUP_FILTER "(&(objectClass=group)(cn=*B_M_Anaesthesiologie*))"

int ld_conn(LDConn *ldconn) {
	int rc;
	
	if(ldap_initialize(&(ldconn->ld), ldconn->uri)) {
		perror("ldap_initialize");
		return 1;
	}
	
	rc = ldap_simple_bind_s(ldconn->ld, ldconn->binddn, ldconn->pass);
	if(rc != LDAP_SUCCESS) {
		fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(rc));
		return 2;
	}
	printf( "Successfully authenticated\n" );
	
	return 0;
}

/**
 * fixme: adressing wrong bytes I guess
 * http://stackoverflow.com/questions/1672677/print-a-guid-variable
 * https://packages.debian.org/jessie/libuuid1
 */
char *guid2str(const unsigned char *raw) {
	/*
	uuid.h

	#include "copyrt.h"
	#undef uuid_t
	typedef struct {
		  unsigned32  time_low;
		  unsigned16  time_mid;
		  unsigned16  time_hi_and_version;
		  unsigned8   clock_seq_hi_and_reserved;
		  unsigned8   clock_seq_low;
		  byte        node[6];
	} uuid_t;
	
	// puid -- print a UUID 
	void puid(uuid_t u)
	{
		  int i;

		  printf("%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
		  u.time_hi_and_version, u.clock_seq_hi_and_reserved,
		  u.clock_seq_low);
		  for (i = 0; i < 6; i++)
		      printf("%2.2x", u.node[i]);
		  printf("\n");
	}
	typedef struct {
		  uint32_t  time_low;
		  uint16_t  time_mid;
		  uint16_t  time_hi_and_version;
		  uint8_t   clock_seq_hi_and_reserved;
		  uint8_t   clock_seq_low;
		  uint8_t   node[6];
	} uuid_t;
	
	uuid_t *u;
	u = malloc(sizeof(uuid_t));
	memcpy((void *) u->time_low, (const void *) raw[0], 4);
	memcpy((void *) u->time_mid, (char *) raw[4], 2);
	memcpy((void *) u->time_hi_and_version, (char *) raw[6], 2);
	memcpy((void *) u->clock_seq_hi_and_reserved, (char *) raw[8], 1);
	memcpy((void *) u->clock_seq_low, (char *) raw[9], 1);
	
	u->node[0] = raw[10];
	u->node[1] = raw[11];
	u->node[2] = raw[12];
	u->node[3] = raw[13];
	u->node[4] = raw[14];
	u->node[5] = raw[15];
	*/
	
	char str[40];
	
	sprintf(str, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
		raw[3], raw[2], raw[1], raw[0], raw[5], raw[4],
		raw[7], raw[6], raw[8], raw[9], raw[10], raw[11],
		raw[12], raw[13], raw[14], raw[15]
	);
	
	/*
	sprintf(str, 
		"%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x", u->time_low, u->time_mid,
		  u->time_hi_and_version, u->clock_seq_hi_and_reserved,
		  u->clock_seq_low, u->node[0], u->node[1], u->node[2], u->node[3], u->node[4], u->node[5]
	);
	*/
	
	return str;
}

void ldap_display_attributes(LDAP *ld, LDAPMessage *msg) {
	// get all attributes
	char *a;
	BerElement *ber;
	
	for (a = ldap_first_attribute(ld, msg, &ber); 
		a != NULL; 
		a = ldap_next_attribute(ld, msg, ber)) {
		
		// skip all non objectGUID attributes for testing
		// if (strcmp(a, "objectGUID") != 0) continue; 
		
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
	LDAP        *ld;
	int         rc;
	char        bind_dn[254];
	LDAPMessage *res, *msg;
	int         msgtype;
	char        *dn;
	LDConn      *conn; // = (LDConn*) malloc(sizeof(LDConn));
	int         num_entries;
	int         num_refs;
	
	// setup our connection data structure
	sprintf(bind_dn, LDAP_BINDDN, LDAP_USER);
	
	conn->binddn = bind_dn;
	conn->pass = LDAP_PASS;
	conn->search_base = LDAP_SEARCH_BASE;
	conn->search_filter = LDAP_GROUP_FILTER;
	conn->uri = LDAP_SERVER;
	
	// prepare the connection
 	ld_conn(conn);
 	if (conn->ld == 0) {
		fprintf(stderr, "failed to connect, aborting.\n");
 		return 1;
 	}
	
	// fetch result
	rc = ldap_search_s(conn->ld, conn->search_base, LDAP_SCOPE_SUBTREE, 
	                   conn->search_filter, NULL, 0, &(conn->res));
	if (rc != LDAP_SUCCESS) {
		if (ld != NULL) {
			ldap_unbind_ext(conn->ld, NULL, NULL);
			printf("Disconnected.\n");
		}
		
		fprintf(stderr, "ldap_simple_bind_s: %s\n", ldap_err2string(rc));
		return 0;
	}
	
	// loop over result
	num_entries = ldap_count_entries(conn->ld, conn->res);
	num_refs = ldap_count_references(conn->ld, conn->res);
	printf("Number of entried: %d, number of References: %d\n", num_entries, num_refs);
	
	for ( msg = ldap_first_message( conn->ld, conn->res ); 
		msg != NULL; 
		msg = ldap_next_message( conn->ld, msg ) ) {

		/* Determine what type of message was sent from the server. */
		msgtype = ldap_msgtype(msg);
		switch( msgtype ) {
			case LDAP_RES_SEARCH_ENTRY:
				// The result is an entry, print dn
				if ( ( dn = ldap_get_dn( conn->ld, msg ) ) != NULL ) {
					printf( "==================================================\n");
					printf( "dn: %s\n", dn );
					/* Free the memory used for the DN when done */
					ldap_memfree( dn );
				}
				
				ldap_display_attributes(conn->ld, msg);
				
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
	if (conn->ld != NULL) {
		ldap_unbind_ext(conn->ld, NULL, NULL);
		printf("Disconnected.\n");
	}
	
	return 0;
}

