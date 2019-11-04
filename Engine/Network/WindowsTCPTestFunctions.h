#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string>
#pragma comment(lib,"Ws2_32.lib")
#define DEFAULT_PORT "3333"
#define DEFAULT_BUFLEN 512
int client() {
	//contain data about the socket
	WSADATA wsaData;
	int iResult;
	//Start winsock DLL
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup Failed: %d\n", iResult);
		return 1;
	}
	//ip address
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	std::string local_host = "127.0.0.1";
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// host address + prot number
	iResult = getaddrinfo(local_host.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Get address infomation Failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;
	//Create the socket
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket():%d \n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	//Connect to the server socket
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	const char* sendbuf = "this is a test";
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	//Send an buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed:%d \n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	printf("Bytes Sent: %d\n", iResult);
	//shutdown the connection for sending, cause no more data to send. However, the client can still recieve data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	// Recieve data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes recieved %d \n", iResult);
		}
		else if (iResult == 0) {
			printf("Connection closed \n");
		}
		else {
			printf("recv failed %d\n", WSAGetLastError());
		}
	} while (iResult > 0);
	//Disconnect and shut down
	//Clean up
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

int server() {
	//Get address infomation
	int iResult;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	//A macro fill a block of memory with zeros, In this case
	//Before doing that hints are stored with rundom values.
	ZeroMemory(&hints, sizeof(hints));
	// IPv4
	hints.ai_family = AF_INET;
	//TCP
	hints.ai_socktype = SOCK_STREAM;
	//TCP
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed %d\n", iResult);
		WSACleanup();
		return 1;
	}
	//Create a SOCKET for the 
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket() : %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	return 0;

}