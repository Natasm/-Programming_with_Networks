from socket import *
import threading, ssl

#working only in unsafe environments

serverPort= 3002
socketServer = socket(AF_INET, SOCK_STREAM)

socketServer.bind(('', serverPort))
socketServer.listen(5)

print 'Ready server to running...'

buffer_size = 10000

def proxy_server(webserver, port, data, connectionSocket) :
    print("Searching server web...")

    try:
        sock = socket(AF_INET, SOCK_STREAM)
        c = ssl.wrap_socket(sock, ssl_version=ssl.PROTOCOL_TLS)
        c.connect((webserver, port))
        c.send(data)

        while 1:
            message = connectionSocket.recv(buffer_size)
            if len(message) > 0: connectionSocket.sendall(message)
            else: break

        c.close()

    finally:
        connectionSocket.close();


def thread_proxy(connectionSocket): 
    try:
        message = connectionSocket.recv(buffer_size)
        first_line = message.decode("utf-8").split("\n")[0]
        print("First line -> " + first_line)

        url = first_line.split(" ")[1]
        
        http_position = url.find('://')
        if http_position == -1 : temp = url
        else: temp = url[(http_position + 3)]

        webserver_position = temp.find('/')
        if webserver_position == -1:  webserver_position = len(temp)

        port_position = temp.find(':')
        if(port_position == -1 or webserver_position < port_position):
            port = 80
            webserver = temp[:webserver_position]
        else:
            port = int(temp[(port_position + 1):][:webserver_position - port_position - 1])
            webserver = temp[:port_position]

        #print("WebServer ->" + webserver)

        proxy_server(webserver, port, message, connectionSocket)

    except IOError:
        print('Illegal request')
        connectionSocket.close()

while True:
    connectionSocket, addr = socketServer.accept()

    print("Conection estabilished...")

    x = threading.Thread(target=thread_proxy, args=(connectionSocket,))
    x.setDaemon(True)
    x.start()

socketServer.close()
