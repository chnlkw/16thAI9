#!/bin/python

PORT = 33444

import time

if __name__ == '__main__':
	from socket import *
	sock = socket(AF_INET, SOCK_STREAM)
	sock.bind(('localhost', PORT))
	sock.listen(2)
	cons = []
	while len(cons) < 2:
		conn, addr = sock.accept()
		name = conn.recv(1024)
		print name, 'logged in'
		conn.send('OK!!')
		print conn
		cons.append(conn)

	for c in cons :
		c.send('Start')
		c.settimeout(0.01)
	t = 0
	while True:
		time.sleep(0.1)
		try:
			for c in cons :
				c.send("now " + str(t))
			allmsg = 'serv ^^ '
			for c in cons :
				s = c.recv(1024)
				print t, ': --- ', s, ' ---'
				allmsg += '(' + s + ') '
			for c in cons :
				c.send(allmsg)
		except error, msg:
			print 'failed :' , msg
			break
		t = t + 1
	for c in cons :
		c.close()
	sock.close()
