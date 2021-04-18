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
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(MAKEWORD(2, 2), &data);
    
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int OptVal = 1;
    int OptLen = sizeof(int);
    int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&OptVal, OptLen);
    string ipAdress = "127.0.0.1";

    sockaddr_in server_addr, client_addr;

    int server_len = sizeof(server_addr);
    ZeroMemory(&server_addr, server_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ipAdress.c_str(), &server_addr.sin_addr);

    int client_len = sizeof(client_addr);
    ZeroMemory(&client_addr, client_len);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    client_addr.sin_addr.S_un.S_addr = INADDR_ANY;

    rc = bind(sock, (struct sockaddr*)&client_addr, client_len);

    char buf[BUFSIZE];
    string userInput;

    do
    {
        //Prompt to type some text
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0)
        {
            int sendResult = sendto(sock, userInput.c_str(), BUFSIZE, 0, (sockaddr*)&server_addr, server_len);
        }
    } while (userInput.size() > 0);

    cin.get();
    closesocket(sock);
    WSACleanup();
    return 0;
}