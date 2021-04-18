#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define BUFSIZE         1024    //buffer
#define SERVER_PORT     54000   //server port #

int main(void)
{
    int rc;
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int OptVal = 1;
    int OptLen = sizeof(int);
    rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&OptVal, OptLen);

    struct sockaddr_in server_addr;
    int server_len = sizeof(server_addr);
    ZeroMemory(&server_addr, server_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

    rc = bind(sock, (struct sockaddr*)&server_addr, server_len);

    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    ZeroMemory(&client_addr, client_len);
    
    char buf[BUFSIZE];
    string userInput;

    while (true)
    {
        ZeroMemory(buf, BUFSIZE);

        int bytesRecieved = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&client_addr, &client_len);

        char clientIP[256];
        ZeroMemory(clientIP, 256);

        inet_ntop(AF_INET, &client_addr.sin_addr, clientIP, 256);
        cerr << "Msg from " << clientIP << " : " << ntohs(client_addr.sin_port) << " $ " << buf << endl;
        int sendResult = sendto(sock, buf, BUFSIZE, 0, (sockaddr*)&client_addr, client_len);
    }

    cin.get();
    closesocket(sock);
    WSACleanup();
    return 0;
}