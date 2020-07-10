import random
from socket import *

serverPort = 3001

serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))

while True:
	rand = random.randint(0, 10)
	
	message, address = serverSocket.recvfrom(1024)
	message = message.upper()

	if rand < 4: continue

	serverSocket.sendto(message, address)