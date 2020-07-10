from socket import *
import base64, ssl

# Configurations 

mailServer = "smtp.gmail.com"
portServer = 465

mailMail = "testeredesmestrado@gmail.com"
#password do mailMail: testeredesmestrado123
mailRCPT = "lemuelsantanademorais@gmail.com"

socket = socket(AF_INET, SOCK_STREAM)
clientSocket = ssl.wrap_socket(socket, ssl_version=ssl.PROTOCOL_TLS)
clientSocket.connect((mailServer, portServer)) 

recv = clientSocket.recv(1024)
print recv
if recv[:3] != '220':
	print '220 reply not received from server.'
	exit()

helloCommand = 'HELO Natan\r\n' #The client sends this command to the SMTP server to identify itself and initiate the SMTP conversation
clientSocket.send(helloCommand)
recv = clientSocket.recv(1024)
print recv
if recv[:3] != '250':
	print '250 reply not received from server.'
	exit()

# authenticate
username = mailMail
password = raw_input('Input password of the email: ')
base64_str = ('\x00'+username+'\x00'+password).encode()
base64_str = base64.b64encode(base64_str)
authCommand = "AUTH PLAIN ".encode()+base64_str+"\r\n".encode()
print(authCommand)
clientSocket.send(authCommand)
recv = clientSocket.recv(1024)
print recv.decode()


# Verify emails

fromCommand = 'MAIL FROM: <' + mailMail + '>\r\n'
print(fromCommand)
clientSocket.send(fromCommand)
recv = clientSocket.recv(1024)
print recv

rcptCommand = 'RCPT TO:<' + mailRCPT + '>\r\n'
clientSocket.send(rcptCommand)
recv = clientSocket.recv(1024)
print recv

# Send coomand DATA - This command starts the transfer of the message 
dataCommand = 'DATA\r\n'
clientSocket.send(dataCommand)
recv = clientSocket.recv(1024)
print recv

# Send message data.
msg = "\r\n Hello World, Lemuel hahahahahahaha!"
endmsg = "\r\n.\r\n"
data = 'From:' + mailMail + '\r\nTo:' + mailRCPT + '\r\nSubject:Welcome!\r\n' + msg
clientSocket.send(data)

clientSocket.send(endmsg)
recv = clientSocket.recv(1024)
print recv

# Send QUIT command and get server response.
clientSocket.send('quit\r\n')
recv = clientSocket.recv(1024)
print recv
