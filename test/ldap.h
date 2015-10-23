

char *guid2str(const unsigned char *raw);
LDAP *ldap_connect(char* uri, char* binddn, char* bind_pw);
void ldap_display_attributes(LDAP *ld, LDAPMessage *msg);

