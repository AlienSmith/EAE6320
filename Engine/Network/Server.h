#pragma once
#include "Client.h"
#include <mutex>
#include <vector>
#include "stack.h"
#define MAX_CLIENT_NUMBER 2
#include "InputWrapper.h"
namespace Network {
	namespace TCP {
		enum class Server_Phase {
			DISTRIBUTE_ID,
			GAMELOOP_UPDATE,
			INVALID,
		};
		class Server {
		public:
			const static std::string REQUEST_ID;
			Server();
			bool Run(const std::string& port_number, network_error_code& o_error_code);
			bool Start(const std::string& port_number, network_error_code& o_error_code);
			bool Accept(network_error_code& o_error_code, std::shared_ptr<SOCKET>& socket);
			bool Send(const char* data, network_error_code& o_error_code,int str_length, const std::shared_ptr<SOCKET>& socket);
			bool Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code, const std::shared_ptr<SOCKET>& socket);
			bool Recieve(char* o_data, network_error_code& o_error_code, const std::shared_ptr<SOCKET>& socket,int str_length);
			bool Send(UpdateStruct* data, network_error_code& o_error_code, int str_length, const std::shared_ptr<SOCKET>& socket);
			bool Recieve(InputWrapper<InputStruct>* o_data, network_error_code& o_error_code, const std::shared_ptr<SOCKET>& socket);
			bool IntepretRequest(network_error_code& o_error_code, const std::shared_ptr<SOCKET>& socket);
			void Reset();
		private:
#ifdef _WIN32
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_Listen_Socket;
#endif
			std::vector<std::thread> m_thread_pool;
			threadsafe_stack<std::shared_ptr<SOCKET>> m_socket_pool;
			int m_num_client;
			int max_clients_num;
			Server_Phase m_Phase;
			std::mutex num_client_mutex;
		};
	}
}