#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// link program with Winsock library
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET clientSocketFD;
    struct sockaddr_in serverAddress;
    const char *DATACONTENT = "Aughhhhh";

    // initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("initialize Winsock -> {Failed}\n");
        return 1;
    }

    // create a socket
    clientSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocketFD == INVALID_SOCKET) {
        printf("create a socket -> {Failed}\n");
        WSACleanup();
        return 1;
    }

    // setup the address information
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connect to the server
    if (connect(clientSocketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("connect to the server -> {Failed}\n");
        closesocket(clientSocketFD);
        WSACleanup();
        return 1;
    }

    // send data to the server
    if (send(clientSocketFD, DATACONTENT, (int)strlen(DATACONTENT), 0) == SOCKET_ERROR) {
        printf("send data to the server -> {Failed}\n");
        closesocket(clientSocketFD);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(clientSocketFD);
    WSACleanup();

    return 0;
}
