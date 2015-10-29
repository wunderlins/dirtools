#!/usr/bin/env python

import ldap, ldif, sys, re
import pprint
from datetime import datetime
import ldap.filter
import csv
import socket
import imp

"""
config = {
	"host": "ms.uhbs.ch",
	"port": None,
	"user": "muana",
	"pass": None,
	"base": "dc=ms,dc=uhbs,dc=ch", # where to look for computers
	"search_filter": "(objectClass=user)", # search for members in these groups
	"vdlist" : "import/VDsMQAssignedUser-10-15.csv"
};
"""

config = {
	"uri": None,
	"binddn": None,
	"pass": None,
	"base": "dc=ms,dc=uhbs,dc=ch", # where to look for computers
	"search_filter": "(objectClass=user)", # search for members in these groups
	"vdlist" : "import/VDsMQAssignedUser-10-15.csv"
};


def get_ma():
	l = None
	
	"""
	if config["port"]:
		l = ldap.open(config["host"], config["port"])
	else:
		l = ldap.open(config["host"])
	
	l.protocol_version = ldap.VERSION3
	username = "cn=" + config["user"] +",ou=GenericMove,ou=Users,ou=USB,dc=ms,dc=uhbs,dc=ch"
	l.simple_bind_s(username, config["pass"])
	"""
	
	
	
	l = ldap.initialize(config["uri"]+"/")
	l.protocol_version = ldap.VERSION3
	print config["uri"]
	print config["binddn"]
	print config["pass"]
	l.simple_bind_s(config["binddn"], config["pass"])
	
	print l
	return 0
	
	"""
	Get All users from:
	- CN=MQ_ANA_AA,OU=Exchange_Adressbuecher,OU=PITServer,DC=ms,DC=uhbs,DC=ch
	- CN=MQ_ANA_AA,OU=Exchange_Adressbuecher,OU=PITServer,DC=ms,DC=uhbs,DC=ch
	- CN=MQ_ANA_AA,OU=Exchange_Adressbuecher,OU=PITServer,DC=ms,DC=uhbs,DC=ch
	"""
	cnlist = []
	
	# find groups of interest
	f = "(|(cn=MQ_ANA_AA)(cn=MQ_ANA_OA)(cn=MQ_ANA_LA))"
	base = "OU=Exchange_Adressbuecher,OU=PITServer,"+config["base"]
	res = l.search_s(base, ldap.SCOPE_SUBTREE, f)
	for r in res:
		#print r[0]
		cnlist = cnlist + r[1]["member"]
		#group_filter += ")(memberOf=" + r[0]
	
	result = []
	aduser = []
	dnlist = []
	#cnlist = ["cn=wunderlin*"]
	for u in cnlist:
		#f = u.replace("\\", "\\\\")
		#f = u.replace("\\,", "*")
		#f = f.replace("(", "")
		#f = f.replace(")", "")
		#f = ldap.filter.escape_filter_chars(u, 0)#.replace("\\5c", "\\", 1)
		#f = u.replace("\\,", "*")
		f = re.sub(r'([^\\]),.*', "\\1", u)
		f = f.replace("\\,", ",")
		cn = f
		dn = u
		f = ldap.filter.escape_filter_chars(f, 0);
		
		res = l.search_s("OU=USB,"+config["base"], ldap.SCOPE_SUBTREE, f)
		samaccountname = None
		try:
			#print res[0][1]["sAMAccountName"][0]
			samaccountname = res[0][1]["sAMAccountName"][0]
		except:
			pass
		
		result.append( (u, cn, samaccountname) )
		aduser.append(samaccountname)
		dnlist.append(dn)
		
	#print cnlist
	#print aduser
	
	vdlist = []
	#read csv file
	with open(config["vdlist"], 'rb') as csvfile:
		hostreader = csv.reader(csvfile, delimiter=',', quotechar='"')
		for row in hostreader:
			adname = ""
			try:
				adname = row[3].split("\\")[1]
			except:
				pass
				
			if adname not in aduser:
				continue
			
			ix = aduser.index(adname)
			print row[0], " ", adname, dnlist[ix]
			
	
def main():
	get_ma()

if __name__ == "__main__":
	
	# read password
	hostname = socket.gethostname()
	passfile = '../etc/%s.pass' % hostname 
	with open(passfile) as f: 
		config["pass"] = f.read()
		config["pass"] = config["pass"].strip() # remove trailing whitespace
	
	# read config
	def read_cfg():
		sys.path.append('../etc/')
		if hostname == "shell1":
			try:
				from shell1 import *
				config["uri"] = uri
				config["binddn"] = binddn
				config["base"] = basedn
			except:
				print("Make sure the config file '%s' exists.") % ('../etc/'+hostname+".py")
				sys.exit(2)
	
	read_cfg()
	
	main()

