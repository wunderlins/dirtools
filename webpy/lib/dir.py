#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import socket
import ldap

# module variables
basepath = None
config = {}
_conn = None

def disconnect():
	global _conn
	_conn.unbind_s()

def connect():
	global _conn
	
	_conn = ldap.initialize(config["uri"])
	_conn.protocol_version = ldap.VERSION3
	_conn.simple_bind_s(config["binddn"], config["pass"])
	
	#print _conn
	return _conn

	
def search(conn, filter):
	return conn.search_s(config["basedn"], ldap.SCOPE_SUBTREE, filter)

def listdn(result):
	for r in result:
		print r[0]

def get_config():
	if config == {}:
		return None
	return config

def init():
	global basepath, config
	basepath = os.path.dirname(os.path.realpath(__file__)) + "/"
	hostname = socket.gethostname()
	passfile = os.path.realpath(basepath+'../../etc/%s.pass' % hostname)
	configpath = os.path.realpath(basepath+'../../etc')
	configfile = os.path.realpath(configpath+'/%s.py' % hostname)
	
	sys.path.append(configpath)
	try:
		cfgfile = __import__(hostname)
	except:
		msg = "Failed to read config file '%s', make sure it exists." % (configfile)
		sys.stderr.write(msg+"\n")
		sys.exit(2)
	
	attrs = [a for a in dir(cfgfile) if not a.startswith('__')]
	for a in attrs:
		v = getattr(cfgfile, a)
		#print "%s=%s" % (a, v)
		config[a] = v
	
	try:
		with open(passfile) as f: 
			config["pass"] = f.read()
			config["pass"] = config["pass"].strip() # remove trailing whitespace
	except:
		print("Error while loading password file %s") % passfile
		sys.exit(3)
		
	#print config

def test(argv):
	#print get_config()
	init()
	#print get_config()
	conn = connect()
	filter = "(|(objectClass=group)(objectClass=posixGroup)(objectClass=organizationalUnit))"
	res  = search(conn, filter)
	listdn(res)
	disconnect()
	

if __name__ == "__main__":
	test(sys.argv)

