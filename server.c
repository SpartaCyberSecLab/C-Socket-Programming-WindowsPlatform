#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// link program with Winsock library
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET serverSocketFD, clientSocketFD;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[512];
    int receivedBytes;
    int clientAddressSize = sizeof(clientAddress);

    // initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("initialize Winsock -> {Failed}\n");
        return 1;
    }

    // create a socket
    serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketFD == INVALID_SOCKET) {
        printf("create a socket -> {Failed}\n");
        WSACleanup();
        return 1;
    }

    // setup the address information
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
    if (bind(serverSocketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("bind the socket -> {Failed}\n");
        closesocket(serverSocketFD);
        WSACleanup();
        return 1;
    }

    // listen for connections
    if (listen(serverSocketFD, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen for connections -> {Failed}\n");
        closesocket(serverSocketFD);
        WSACleanup();
        return 1;
    }

    printf("server is listening on port 8000...\n");

    // accept a client connection
    clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocketFD == INVALID_SOCKET) {
        printf("accept a client connection -> {Failed}\n");
        closesocket(serverSocketFD);
        WSACleanup();
        return 1;
    }

    printf("client connected.\n");

    // receive data from the client
    receivedBytes = recv(clientSocketFD, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes > 0) {
        buffer[receivedBytes] = '\0';
        printf("received data: %s\n", buffer);
        send(clientSocketFD, buffer, receivedBytes, 0);
    } else if (receivedBytes == 0) {
        printf("connection closed by client.\n");
    } else {
        printf("receive data from the client -> {Failed}\n");
    }

    // clean up
    closesocket(clientSocketFD);
    closesocket(serverSocketFD);
    WSACleanup();

    return 0;
}
