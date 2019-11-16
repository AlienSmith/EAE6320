#include "Client.h"

Network::TCP::Client::Client() :m_wsaData(), m_result(NULL), m_ptr(NULL), m_hints(),m_socket(INVALID_SOCKET),id(0) {}

bool Network::TCP::Client::Obtain_id(const std::string& host, const std::string& port_number,network_error_code& o_error_code)
{
	char* temp = nullptr;
	if (Connect(host, port_number, o_error_code)) {
		if (Send("REQUEST_ID", o_error_code)) {
			if (Recieve(temp, o_error_code)) {
				id = *(reinterpret_cast<int*> (temp));
				printf("My id is %d", id);
				return true;
			}
		}
	}
	return false;
}

bool Network::TCP::Client::Connect(const std::string& host, const std::string& port_number, network_error_code& o_error_code)
{
	int iResult;
	//Start winsock DLL
	iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iResult != 0) {
		o_error_code.code = "WSAStartup Failed: %d\n";
		return false;
	}
	ZeroMemory(&m_hints, sizeof(m_hints));
	m_hints.ai_family = AF_UNSPEC;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo(host.c_str(), port_number.c_str(), &m_hints, &m_result);
	if (iResult != 0) {
		o_error_code.code = "Get address infomation Failed: %d\n";
		WSACleanup();
		return false;
	}
	m_ptr = m_result;
	//Create the socket
	m_socket = socket(m_ptr->ai_family, m_ptr->ai_socktype, m_ptr->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		o_error_code.code = "Error at socket() \n";
		freeaddrinfo(m_result);
		WSACleanup();
		return false;
	}
	//Connect to the server socket
	iResult = connect(m_socket, m_ptr->ai_addr, (int)m_ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	freeaddrinfo(m_result);
	if (m_socket == INVALID_SOCKET) {
		o_error_code.code = "Unable to connect to server!\n";
		WSACleanup();
		return false;
	}
	return true;
}

bool Network::TCP::Client::Send(const char* data, network_error_code& o_error_code)
{
	int iResult;
	//Send an buffer notice here used strlen to measure the length of the data.
	iResult = send(m_socket, data, (int)strlen(data)+1, 0);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "send failed:%d \n";
		closesocket(m_socket);
		WSACleanup();
		return false;
	}
	printf("Bytes Sent: %d\n", iResult);
	//shutdown the connection for sending, cause no more data to send. However, the client can still recieve data
	iResult = shutdown(m_socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "shutdown sending failed: \n";
		closesocket(m_socket);
		WSACleanup();
		return false;
	}
	return true;
}

int Network::TCP::Client::Recieve(char* o_data, network_error_code& o_error_code)
{
	int iResult;
	int recvbuf_length = 100;
	char recvbuf[100];
	int Byte_recieved = 0;
	do {
		iResult = recv(m_socket, recvbuf, 100, 0);
		if (iResult > 0) {
			printf("Byte recieved %d \n", iResult);
			Byte_recieved += iResult;
		}
		else if (iResult == 0) {
			printf("Connection Closed \n");
			o_data = recvbuf;
		}
		else {
			o_error_code.code = "recv failed %d\n";
			return 0;
		}
	} while (iResult > 0);
	return iResult;
}

void Network::TCP::Client::Reset()
{
	closesocket(m_socket);
	WSACleanup();
	m_wsaData = WSADATA();
	m_result = NULL;
	m_ptr = NULL;
	m_hints = addrinfo();
	m_socket = INVALID_SOCKET;
}



