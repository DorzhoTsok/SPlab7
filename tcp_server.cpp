#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")
#define MY_PORT 666

int main(int argc, char* argv[])
{
    char buff[1024];
    WSAStartup(0x0202, (WSADATA*)&buff[0]);

    SOCKET mysocket;
    // AF_INET - сокет Интернета
    // SOCK_STREAM - сокет для TCP
    // SOCK_DGRAN - сокет для UDP
    // 0 - по умолчанию выбирается TCP протокол
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(MY_PORT);
    local_addr.sin_addr.s_addr = 0;

    bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr));

    listen(mysocket, 1);

    SOCKET client_socket;
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, &client_addr_size)))
    {
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

        printf("+%s [%s] new connect!\n", (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
        char buff[1024];
        send(client_socket, "hello\n", sizeof("hello\n"), 0);

        int bytes_recv;
        while ((bytes_recv = recv(client_socket, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {
            buff[bytes_recv] = 0;
            printf("Client said: %s", buff);
            send(client_socket, buff, bytes_recv, 0);
        }

        closesocket(client_socket);
    }
    return 0;
}
