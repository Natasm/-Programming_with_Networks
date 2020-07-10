import time
from socket import *

serverPort = 3001
serverName = 'localhost'

clientSocket = socket(AF_INET, SOCK_DGRAM)
clientSocket.settimeout(1)

for i in range(1, 11):
	message = 'Ping %d %s' % (i, time.time())
	print message
	t0 = time.time()
	clientSocket.sendto(message, (serverName, serverPort))
	try:
		modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
		print 'from', serverAddress, ":", modifiedMessage, 'time =', (time.time() - t0) * 1000, 'ms\n'
	except:
		print 'Request timed out\n'

clientSocket.close()