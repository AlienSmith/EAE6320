#include "Server.h"

Network::TCP::Server::Server():m_result(NULL), m_ptr(NULL), m_hints(), m_Listen_Socket(INVALID_SOCKET),m_Connect_Socket(INVALID_SOCKET)
{
}

bool Network::TCP::Server::Start(const std::string& port_number, network_error_code& o_error_code)
{
	int iResult;
	ZeroMemory(&m_hints, sizeof(m_hints));
	// IPv4
	m_hints.ai_family = AF_INET;
	//TCP
	m_hints.ai_socktype = SOCK_STREAM;
	//TCP
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, port_number.c_str(), &m_hints, &m_result);
	if (iResult != 0) {
		o_error_code.code = "getaddrinfo failed %d\n";
		WSACleanup();
		return false;
	}
	//Initialize listensocket
	m_Listen_Socket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
	if (m_Listen_Socket == INVALID_SOCKET) {
		o_error_code.code = "Error at socket()\n";
		WSACleanup();
		return false;
	}
	//Bind listensocket
	iResult = bind(m_Listen_Socket, m_result->ai_addr, (int)m_result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "bind failed with error:\n";
		freeaddrinfo(m_result);
		closesocket(m_Listen_Socket);
		WSACleanup();
		return false;
	}
	freeaddrinfo(m_result);
	if (listen(m_Listen_Socket, SOMAXCONN) == SOCKET_ERROR) {
		o_error_code.code = "Listen failed with error \n";
		closesocket(m_Listen_Socket);
		WSACleanup();
		return false;
	}
	//Acception connection from client
	printf("Waiting for connection.\n");
	return true;
}

bool Network::TCP::Server::accept()
{
	return false;
}
