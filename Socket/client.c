#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <process.h>

#define BUFFER_SIZE 255

char buffer_send[BUFFER_SIZE];
char buffer_recv[BUFFER_SIZE];

int newsocket(char* remote_ip, unsigned short remote_port, struct sockaddr_in remote_address){
    int remote_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (remote_socket != INVALID_SOCKET) printf("Socket criado com sucesso!\n");
    else {
        printf("Criação do socket falhou!");
        return -1;
    }

    memset(&remote_address, 0, sizeof(remote_address));

    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(remote_port);
    remote_address.sin_addr.s_addr = inet_addr(remote_ip);

    if(connect(remote_socket, (struct sockaddr *) &remote_address, sizeof(remote_address)) != SOCKET_ERROR) printf("Conexão com o servidor realizado com sucesso!\n\n");
    else {
        printf("Falha na conexão!");
        return -1;
    }
    return remote_socket;
}

unsigned int __stdcall sendMessageSocket(void* socket_client){
    int remote_socket = (int) socket_client;

    while(1){
        memset(&buffer_send, 0, BUFFER_SIZE);
        printf("Digite sua mensagem: ");
        gets(buffer_send);
        fflush(stdin);

        if(buffer_send[0] == '#') break;

        if(send(remote_socket, buffer_send, BUFFER_SIZE, 0) != SOCKET_ERROR) printf("Mensagem enviada!\n");
        else printf("Mensagem não foi enviada!\n");
    }

    return 0;
}

unsigned int __stdcall recvMessageSocket(void* socket_client){
    int socket_remote = (int) socket_client;

    while(1){
        memset(&buffer_recv, 0, sizeof(BUFFER_SIZE));

        int message_length = recv(socket_remote, buffer_recv, BUFFER_SIZE, 0);
        if(message_length != SOCKET_ERROR) printf("\nMensagem recebida: %s\n", buffer_recv);
    }

    return 0;
}

int main(){
    WSADATA wsadata;

    struct sockaddr_in remote_address;

    unsigned short remote_port = 0;

    char remote_ip[32];

    printf("IP do servidor: ");
    scanf("%s", remote_ip);
    fflush(stdin);

    printf("Porta do servidor: ");
    scanf("%d", &remote_port);
    printf("Porta %d", remote_port);
    fflush(stdin);

    if(WSAStartup(MAKEWORD(2,0), &wsadata) == 0){
        printf("WinSock iniciado.\n");
    }
    else {
        printf("WinSock falhou.");
        return 0;
    }

    int remote_socket = newsocket(remote_ip, remote_port, remote_address);

    _beginthreadex(0, 0, recvMessageSocket, (void*) remote_socket, 0, 0);
    _beginthreadex(0, 0, sendMessageSocket, (void*) remote_socket, 0, 0);

    while(1) {}

    printf("\nCliente encerrado!\n");
    WSACleanup();
}
