from socket import *

serverPort = 3000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)

print('Server ready to running...')

while True:
	connectionSocket, addr = serverSocket.accept();

	if connectionSocket:
		print('Connection established...')
		try:
			req = connectionSocket.recv(1024)

			if len(req) > 0 :
				filename = req.split()[1]
				f = open(filename[1:])
				outputdata = f.read()
				connectionSocket.send('HTTP/1.1 200 ok\r\n')
				connectionSocket.send('Content-Type: text/html; charset=utf-8\r\n\n')
				for i in range(0, len(outputdata)):
					connectionSocket.send(outputdata[i])
				f.close()

		except IOError:
			print('Error main page!')

			try:
				f = open('not_found.html')
				outputdata = f.read()
				connectionSocket.send('HTTP/1.1 200 ok\r\n')
				connectionSocket.send('Content-Type: text/html; charset=utf-8\r\n\n')
				for i in range(0, len(outputdata)):
					connectionSocket.send(outputdata[i])
				f.close()
			
			except IOError:
				print('Error 404 page')
		
		finally:
			connectionSocket.close()

serverSocket.close()