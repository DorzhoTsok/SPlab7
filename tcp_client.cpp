#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")
#define PORT 666
#define SERVERADDR "127.0.0.1"

int main(int argc, char* argv[])
{
    char buff[1024];
    printf("TCP DEMO CLIENT\n");
    WSAStartup(0x202, (WSADATA*)&buff[0]);

    SOCKET my_sock;
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT* hst;

    dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);

    connect(my_sock, (sockaddr*)&dest_addr, sizeof(dest_addr));

    int nsize;
    nsize = recv(my_sock, buff, sizeof(buff) - 1, 0);
    buff[nsize] = 0;
    printf("S=>C:%s", buff);
    printf("S<=C:");
    fgets(buff, sizeof(buff) - 1, stdin);

    send(my_sock, buff, strlen(&buff[0]), 0);
    nsize = recv(my_sock, buff, sizeof(buff) - 1, 0);
    buff[nsize] = 0;
    printf("S=>C:%s", buff);
    closesocket(my_sock);
    WSACleanup();
    return -1;
}