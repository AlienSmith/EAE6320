#include "Server.h"
const std::string Network::TCP::Server::REQUEST_ID = std::string("REQUEST_ID");
Network::TCP::Server::Server():m_result(NULL), m_ptr(NULL), m_hints(), m_Listen_Socket(INVALID_SOCKET),m_num_client(0),m_thread_pool(),m_socket_pool()
{
}

bool Network::TCP::Server::Run(const std::string& port_number, network_error_code& o_error_code)
{
	char* recv_data = nullptr;
	if(Start(port_number, o_error_code)) {
		std::shared_ptr<SOCKET> temp(new SOCKET());
		*temp = INVALID_SOCKET;
		if (Accept(o_error_code,temp)) {
			/*std::thread temp_thread{ [this]() {
				Network::network_error_code temp_code;
				if (IntepretRequest(temp_code)) {

				}
			} };*/
			if (IntepretRequest(o_error_code,temp)) {
				return true;
			}
		}
	}
	return false;
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

bool Network::TCP::Server::Accept(network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket)
{
	*socket = accept(m_Listen_Socket, NULL, NULL);
	if (*socket == INVALID_SOCKET) {
		o_error_code.code = "accept fialed %d\n";
		closesocket(m_Listen_Socket);
		WSACleanup();
		return false;
	}
	return true;
}

bool Network::TCP::Server::Send(const char* data, network_error_code& o_error_code, int str_length, std::shared_ptr<SOCKET>& socket)
{
	int iSendResult = send(*socket, data, str_length, 0);
	if (iSendResult == SOCKET_ERROR) {
		o_error_code.code = "send failed: \n";
		closesocket(*socket);
		WSACleanup();
		return false;
	}
	printf("Bytes sent: %d\n", iSendResult);
	iSendResult = shutdown(*socket, SD_SEND);
	if (iSendResult == SOCKET_ERROR) {
		o_error_code.code = "shutdown sending failed: \n";
		closesocket(*socket);
		WSACleanup();
		return false;
	}
	return true;
}

bool Network::TCP::Server::Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket)
{
	int iResult;
	static const int DEFAULT_BUFLENGTH = 512;
	int recvbuflen = 100;
	std::shared_ptr<char[]> recvbuf(new char[recvbuflen]);
	do {
		iResult = recv(*socket, recvbuf.get(), recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes recieved %d\n", iResult);
			printf(recvbuf.get());
		}
		else if (iResult == 0) {
			printf("Connection closing ... \n");
		}
		else {
			o_error_code.code =	"recv failed: \n";
			closesocket(*socket);
			WSACleanup();
			return false;
		}
	} while (iResult > 0);
	o_data = recvbuf;
	iResult = shutdown(*socket, SD_RECEIVE);
	if (iResult == SOCKET_ERROR) {
		o_error_code.code = "shutdown sending failed \n";
		closesocket(*socket);
		WSACleanup();
		return false;
	}
	return true;
}

bool Network::TCP::Server::IntepretRequest(network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket)
{
	std::shared_ptr<char[]> recv_data = nullptr;
		if (Recieve(recv_data, o_error_code,socket)) {
			std::string recived(recv_data.get());
			if (recived == REQUEST_ID) {
				m_num_client++;
				char* send = reinterpret_cast<char*>(&m_num_client);
				int buf_length = sizeof(m_num_client);
				if (Send(send, o_error_code, buf_length, socket)) {
					closesocket(*socket);
					return true;
				}
			}
		}
	return false;
}

void Network::TCP::Server::Reset()
{
	WSACleanup();
	m_result = NULL;
	m_ptr = NULL;
	m_hints = addrinfo();
	m_Listen_Socket = INVALID_SOCKET;
}

