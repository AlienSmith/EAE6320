#pragma once
#include "Client.h"
namespace Network {
	namespace TCP {
		class Server {
		public:
			Server();
			bool Start(const std::string& port_number, network_error_code& o_error_code);
			bool accept();
			bool Send(const char* data, network_error_code& o_error_code);
			bool Recieve(char* o_data, network_error_code& o_error_code);
			void Reset();
			void Main_GameLoop();
		private:
#ifdef _WIN32
			WSADATA m_wsaData;
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_Listen_Socket;
			SOCKET m_Connect_Socket;
#endif
		};
	}
}