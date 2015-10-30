uri = "ldap://ms.uhbs.ch"
binddn = "CN=A_WunderlinS,OU=Admin,OU=Users,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"
basedn = "DC=ms,DC=uhbs,DC=ch"

sync = [
	{"filter": "(& (cn=MQ_ANA_AA) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Aerzte-AA,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},
	{"filter": "(& (cn=MQ_ANA_OA) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Aerzte-OA,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},
	{"filter": "(& (cn=MQ_ANA_LA) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Aerzte-LA,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},

	{"filter": "(& (cn=MQ_ANA_OIB_Aerzte) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Aerzte-OIB,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},
	{"filter": "(& (cn=MQ_ANA_OIB_Pflege) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Pflege-OIB,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},

	{"filter": "(& (cn=MQ_ANA_OPS) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Pflege-OPS,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},
	{"filter": "(& (cn=MQ_ANA_Pflege) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Pflege-ANA,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},

	{"filter": "(& (cn=MQ_ANA_IT) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Admin-IT,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},
	{"filter": "(& (cn=MQ_ANA_SEK) (objectClass=group) )", "to": "CN=MQ_B_M_Anaesthesiologie-VD-Admin-SEK,OU=Business,OU=Groups,OU=MQInf,OU=USB,DC=ms,DC=uhbs,DC=ch"},
]
