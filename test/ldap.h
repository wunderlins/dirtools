/**
 * LDAP Search example
 * Reference: http://www-archive.mozilla.org/directory/csdk-docs/search.htm
 */

typedef struct {
	LDAP *ld;
	char *binddn;
	char *uri;
	char *pass;
	char *search_base;
	char *search_filter;
	LDAPMessage *res;
} LDConn;

// main methods
int ld_conn(LDConn *ldconn);
// int ld_filter_set(LDConn *conn, char *filter);
// int ld_fetch(LDConn *conn); // 	rc = ldap_search_s(conn->ld, conn->search_base, LDAP_SCOPE_SUBTREE,  conn->search_filter, NULL, 0, &(conn->res)); ldap_first_message( conn->ld, conn->res );
// LDAPMessage *next_result(LDConn *ldc);
// int ld_num_entries(LDConn *conn) {return ldap_count_entries(conn->ld, conn->res);}
// int ld_num_references(LDConn *conn) {return ldap_count_references(conn->ld, conn->res);}
// int ld_free(LDConn *conn)

char *guid2str(const unsigned char *raw);
void ldap_display_attributes(LDAP *ld, LDAPMessage *msg);

