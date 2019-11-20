#include "../Client.h"
Network::TCP::Client::Client() :m_wsaData(), m_result(NULL), m_ptr(NULL), m_hints(),m_socket(INVALID_SOCKET), flag_running(true), m_id(0), m_client_logic(nullptr) {}


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

bool Network::TCP::Client::Send(const char* data, network_error_code& o_error_code, int str_length)
{
	int iResult;
	//Send an buffer notice here used strlen to measure the length of the data.
	iResult = send(m_socket, data, str_length, 0);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "send failed:%d \n";
		closesocket(m_socket);
		WSACleanup();
		return false;
	}
	//printf("Bytes Sent: %d\n", iResult);
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

int Network::TCP::Client::Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code)
{
	int iResult;
	int recvbuf_length = 100;
	std::shared_ptr<char[]> recvbuf(new char[recvbuf_length]);
	int Byte_recieved = 0;
	do {
		iResult = recv(m_socket, recvbuf.get(), 100, 0);
		if (iResult > 0) {
			//printf("Byte recieved %d \n", iResult);
			Byte_recieved += iResult;
		}
		else if (iResult == 0) {
			//printf("Connection Closed \n");
		}
		else {
			o_error_code.code = "recv failed %d\n";
			return -1;
		}
	} while (iResult > 0);
	o_data = recvbuf;
	iResult = shutdown(m_socket, SD_RECEIVE);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "shutdown sending failed \n";
		closesocket(m_socket);
		WSACleanup();
		return -1;
	}
	return Byte_recieved;
}

bool Network::TCP::Client::Recieve(char* o_data, network_error_code& o_error_code, int str_length)
{
	int iResult;
	do {
		iResult = recv(m_socket, o_data, str_length, 0);
		if (iResult > 0) {
			//printf("Byte recieved %d \n", iResult);
		}
		else if (iResult == 0) {
			//printf("Connection Closed \n");
		}
		else {
			o_error_code.code = "recv failed %d\n";
			return false;
		}
	} while (iResult > 0);
	iResult = shutdown(m_socket, SD_RECEIVE);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "shutdown sending failed \n";
		closesocket(m_socket);
		WSACleanup();
		return false;
	}
	return true;
}

void Network::TCP::Client::Reset()
{
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
		m_wsaData = WSADATA();
		m_result = NULL;
		m_ptr = NULL;
		m_hints = addrinfo();
		m_socket = INVALID_SOCKET;
	}
}



