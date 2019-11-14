#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string>
namespace Network {
	struct network_error_code {
		std::string code;
	};
	namespace TCP {
		class Client {
		public:
			Client();
			bool Obtain_id(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Connect(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Send(const char* data, network_error_code& o_error_code);
			bool Recieve(char* o_data, network_error_code& o_error_code);
			void Reset();
		private:
#ifdef _WIN32
			WSADATA m_wsaData;
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_socket;
#endif
			int id;
		};
	}
}