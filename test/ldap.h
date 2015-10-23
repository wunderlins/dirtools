typedef struct {
	LDAP *ld;
	char *binddn;
	char *uri;
	char *pass;
	char *search_base;
	char *search_filter;
	LDAPMessage *res;
} LDConn;

char *guid2str(const unsigned char *raw);
int ld_conn(LDConn *ldconn);
void ldap_display_attributes(LDAP *ld, LDAPMessage *msg);

