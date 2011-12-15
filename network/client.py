#!/bin/python

SERVER_IP = 'localhost'
SERVER_PORT = 33444

from socket import *
import time
import sys

if __name__ == '__main__':
	player = sys.argv[1]
	sock = socket(AF_INET, SOCK_STREAM)
	sock.connect((SERVER_IP, SERVER_PORT))
	sock.send(player)
	s = '..'
	while s != 'Start' :
		s = sock.recv(1024)
		print s
	try:
		s = '..'
		while s != '':
			s = sock.recv(1024)
			print 'Time stamp:', s
			sock.send('I''m ' + player + ', Running...')
			s = sock.recv(1024)
			print 'New message:', s
			print '---'

	except error, msg:
		print 'ERROR :', msg

	sock.close()
