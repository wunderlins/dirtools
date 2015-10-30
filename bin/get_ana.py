#!/usr/bin/env python

import os
import ldap, ldif, sys, re
import pprint
from datetime import datetime
import ldap.filter
import csv
import socket
import imp

config = {
	"uri": None,
	"binddn": None,
	"pass": None,
	"base": "dc=ms,dc=uhbs,dc=ch", # where to look for computers
	"search_filter": "(objectClass=user)", # search for members in these groups
	"vdlist" : "VDsMQAssignedUser-10-15.csv"
};


def get_ma():
	l = None
	
	l = ldap.initialize(config["uri"])
	l.protocol_version = ldap.VERSION3
	l.simple_bind_s(config["binddn"], config["pass"])
	
	cnlist = []
	
	# find groups of interest
	f = "(|(cn=MQ_ANA_AA)(cn=MQ_ANA_OA)(cn=MQ_ANA_LA))"
	base = "OU=Exchange_Adressbuecher,OU=PITServer,"+config["base"]
	res = l.search_s(base, ldap.SCOPE_SUBTREE, f)
	for r in res:
		cnlist = cnlist + r[1]["member"]
	
	result = [] # list of (user, cn, sAMAccountName)
	aduser = [] # list of sAMAccountName
	dnlist = [] # all dn with a sAMAccountName
	for u in cnlist:
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
	with open(basepath + "../var/" + config["vdlist"], 'rb') as csvfile:
		hostreader = csv.reader(csvfile, delimiter=',', quotechar='"')
		for row in hostreader:
			adname = ""
			try:
				adname = row[3].split("\\")[1]
			except:
				pass
				
			if adname not in aduser:
				continue
			
			# resolve the full dn of the vd
			vddn = None
			try:
				r = l.search_s("OU=USB,"+config["base"], ldap.SCOPE_SUBTREE, "cn="+row[0])
				vddn = r[0][0]
			except:
				pass
			
			ix = aduser.index(adname)
			print "%s;%s" % (vddn, dnlist[ix])
			
def main():
	get_ma()

if __name__ == "__main__":
	
	# read password
	basepath = os.path.dirname(os.path.realpath(__file__)) + "/"
	hostname = socket.gethostname()
	passfile = basepath+'../etc/%s.pass' % hostname 
	try:
		with open(passfile) as f: 
			config["pass"] = f.read()
			config["pass"] = config["pass"].strip() # remove trailing whitespace
	except:
		print("Error while loading password file %s") % passfile
		sys.exit(2)
	
	# read config
	def read_cfg():
		sys.path.append(basepath+'../etc/')
		try:
			#from shell1 import *
			cfgfile = __import__(hostname)
			config["uri"] = cfgfile.uri
			config["binddn"] = cfgfile.binddn
			config["base"] = cfgfile.basedn
		except:
			print("Make sure the config file '%s' exists.") % ('../etc/'+hostname+".py")
			sys.exit(2)
	
	read_cfg()
	
	main()
	
