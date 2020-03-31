#ifndef CLIENT_INFO_H_INCLUDED
#define CLIENT_INFO_H_INCLUDED

struct client_info {
    char name;
    struct sockaddr_in adress;
};

#endif // CLIENT_INFO_H_INCLUDED
