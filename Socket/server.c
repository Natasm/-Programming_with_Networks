#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <process.h>

#define BUFFER_SIZE 255
#define MAX_USERS 10

struct sockaddr_in remote_address;
/*Se não for preciso obter informações do cliente,
  não é necessário esta variável, podendo deixar
  null no accept, por exemplo.
*/

int remote_length = sizeof(remote_address);

char buffer[BUFFER_SIZE];

int list_sockets[MAX_USERS];
int indexUsers = 0;

unsigned int __stdcall clientThreadRotine(void* socket_client){
    int socket_remote = (int) socket_client;

    while(1){
        memset(&buffer, 0, sizeof(BUFFER_SIZE));

        int message_length = recv(socket_remote, buffer, BUFFER_SIZE, 0);

        if (message_length != SOCKET_ERROR){
                printf("\nMensagem recebida: %s\n", buffer);
                for(int i = 0; i < indexUsers; i++) {
                    if(send(list_sockets[i], buffer, BUFFER_SIZE, 0) != SOCKET_ERROR) printf("Mensagem enviada com sucesso!\n");
                }
        }
    }
    return 0;
}

int existSocket(int socket_remote){
    for(int i = 0; i < MAX_USERS; i++) if(list_sockets[i] == socket_remote) return 1;
    return 0;
}

int main(){
    WSADATA wsadata;

    struct sockaddr_in local_address;

    unsigned short local_port = 2000;

    if(WSAStartup(MAKEWORD(2,0), &wsadata) == 0){
        printf("WinSock iniciado.\n");
    }
    else {
        printf("WinSock falhou.");
        return 0;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket != INVALID_SOCKET) printf("Socket criado com sucesso!\n");
    else {
        printf("Criação do socket falhou!");
        return 0;
    }

    memset(&local_address, 0, sizeof(local_address));

    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(local_port);
    local_address.sin_addr.s_addr = inet_addr("192.168.1.5");

    if(bind(server_socket, (struct sockaddr*) &local_address, sizeof(local_address)) != SOCKET_ERROR) printf("Bind realizado com sucesso!\n");
    else {
        printf("Falha no bind!");
        return 0;
    }

    if(listen(server_socket, 3) != SOCKET_ERROR) printf("Listen iniciado com sucesso!\n");
    else {
        printf("Erro de listen!");
        return 0;
    }

    printf("IP do servidor: %s\n\n", inet_ntoa(local_address.sin_addr));
    printf("Porta do servidor: %d\n\n", local_port);

    while(1){
        int socket_remote = accept(server_socket, (struct sockaddr*) &remote_address, &remote_length);
        if(socket_remote != INVALID_SOCKET){
            _beginthreadex(0, 0, clientThreadRotine, (void*) socket_remote, 0, 0);
            list_sockets[indexUsers] = socket_remote;
            indexUsers++;
        }
    }

    printf("\nServer shutdown!\n");
    WSACleanup();
    closesocket(server_socket);
}
