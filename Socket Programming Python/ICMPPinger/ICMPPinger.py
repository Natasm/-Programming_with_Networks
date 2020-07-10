import socket
import os
import sys
import struct
import time
import select
import binascii
ICMP_ECHO_REQUEST = 8


def checksum(str):
	csum = 0
	countTo = (len(str) / 2) * 2
	count = 0
	while count < countTo:
		thisVal = ord(str[count + 1]) * 256 + ord(str[count])
		csum = csum + thisVal
		csum = csum & 0xffffffffL
		count = count + 2
	if countTo < len(str):
		csum = csum + ord(str[len(str) - 1])
		csum = csum & 0xffffffffL
	csum = (csum >> 16) + (csum & 0xffff)
	csum = csum + (csum >> 16)
	answer = ~csum
	answer = answer & 0xffff
	answer = answer >> 8 | (answer << 8 & 0xff00)
	return answer


def receiveOnePing(mySocket, ID, timeout, destAddr):
	timeLeft = timeout
	while 1:
		startedSelect = time.time()
		whatReady = select.select([mySocket], [], [], timeLeft)
		howLongInSelect = (time.time() - startedSelect)
		if whatReady[0] == []: # Timeout
			return "Request timed out."

		timeReceived = time.time()
		recPacket, addr = mySocket.recvfrom(1024)

		# Fetch the ICMP header from the IP packet
		p_id = struct.unpack('bbHHh', recPacket[20:28])[3]
		if p_id == ID:
			return timeReceived - startedSelect

        timeLeft = timeLeft - howLongInSelect
        if timeLeft <= 0:
			return "Request timed out."


def sendOnePing(mySocket, destAddr, ID):
	# Header is type (8), code (8), checksum (16), id (16), sequence (16)
	myChecksum = 0

	header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, myChecksum, ID, 1)
	data = struct.pack("d", time.time())

	myChecksum = checksum(header + data)

	if sys.platform == 'darwin':
		myChecksum = socket.htons(myChecksum) & 0xffff
	else:
		myChecksum = socket.htons(myChecksum)

	header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, myChecksum, ID, 1)
	packet = header + data

	mySocket.sendto(packet, (destAddr, 1)) 


def doOnePing(destAddr, timeout):
	icmp = socket.getprotobyname("icmp")

	mySocket = socket.socket(socket.AF_INET, socket.SOCK_RAW, icmp)

	myID = os.getpid() & 0xFFFF 
	sendOnePing(mySocket, destAddr, myID)
	delay = receiveOnePing(mySocket, myID, timeout, destAddr)
	mySocket.close()
	return delay


def ping(host, timeout=1):
	
	dest = socket.gethostbyname(host)
	print "Pinging " + dest + " using Python:"
	print ""
	while 1 :
		delay = doOnePing(dest, timeout)
		print delay
		time.sleep(1)

ping("www.google.com")