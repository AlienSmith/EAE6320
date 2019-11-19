#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string>
#include<memory>
#include "InputWrapper.h"
#include "IO_Struct.h"
namespace Network {
	struct network_error_code {
		std::string code;
	};
	namespace TCP {
		class Client {
		public:
			Client();
			bool Send(InputStruct* data, network_error_code& o_error_code);
			bool Recieve(UpdateStruct* data, network_error_code& o_error_code);
			bool run(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Obtain_id(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Connect(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Send(const char* data, network_error_code& o_error_code, int str_length);
			// number of bytes recieved 0 if failed
			int Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code);
			bool Recieve(char* o_data, network_error_code& o_error_code,int str_length);
			void Reset();
			void SetLogicClas(ClientLogic* logic);
		private:
#ifdef _WIN32
			WSADATA m_wsaData;
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_socket;
#endif
			int m_id;
			ClientLogic* m_client_logic;
		};
	}
}