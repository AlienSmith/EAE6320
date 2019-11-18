#pragma once
#include "Client.h"
#include <mutex>
#include <vector>
#include "stack.h"
namespace Network {
	namespace TCP {
		class Server {
		public:
			const static std::string REQUEST_ID;
			Server();
			bool Run(const std::string& port_number, network_error_code& o_error_code);
			bool Start(const std::string& port_number, network_error_code& o_error_code);
			bool Accept(network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket);
			bool Send(const char* data, network_error_code& o_error_code,int str_length, std::shared_ptr<SOCKET>& socket);
			bool Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket);
			bool IntepretRequest(network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket);
			void Reset();
		private:
#ifdef _WIN32
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_Listen_Socket;
#endif
			
			int m_num_client;
			std::vector<std::thread> m_thread_pool;
			threadsafe_stack<std::shared_ptr<SOCKET>> m_socket_pool;
			//std::mutex num_client_mutex;
		};
	}
}