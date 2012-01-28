#!/usr/bin/python

PORT = 33445

import socket
import time
import threading
import Queue

queue = Queue.Queue()

names = ['', '']
readys = ['', '']
chars = ['boss', 'flight']

class ThreadSocket(threading.Thread):
	def __init__(self, queue):
		threading.Thread.__init__(self)
		self.queue = queue
	
	def run(self):
		conn, num = queue.get()
		while True:
			msg = conn.recv(1024).split()
			print msg
			if msg[0] == 'READY' :
				readys[num] = 'yes'
				break
			elif msg[0] == 'LOGIN' :
				names[num] = msg[1]
				conn.send(chars[num])
			else :
				break
					
		queue.task_done()	

if __name__ == '__main__':

	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.bind(('localhost', PORT))
	sock.listen(2)

	for i in range(2):
		t = ThreadSocket(queue)
#		t.setDaemon(True)
		t.start()

	cons = []
	for i in range(2):
		conn, addr = sock.accept()
		cons.append(conn)
		queue.put((conn, i))

	queue.join()

	print 'readys: ', readys
	print 'names : ', names
	for c in cons :
		c.send('Start')
		c.settimeout(0.01)
	t = 0
	while True:
		time.sleep(0.1)
		for c in cons :
			c.send("now " + str(t))
		allmsg = 'serv ^^ '
		for c in cons :
			s = c.recv(1024)
			print t, ': --- ', s, ' ---'
			allmsg += '(' + s + ') '
		for c in cons :
			c.send(allmsg)
		t = t + 1
	for c in cons :
		c.close()
	sock.close()
