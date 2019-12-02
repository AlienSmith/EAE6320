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
			Server();
			bool Run(const std::string& port_number, network_error_code& o_error_code);
			bool Send(UpdateStruct* data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket);
			bool Recieve(InputWrapper<InputStruct>* o_data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket);
			bool IntepretRequest(network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket);
			void SetServerLogic(ServerLogic* serverlogic);
		private:
			bool Start(const std::string& port_number, network_error_code& o_error_code, SOCK& listener);
			bool Accept(network_error_code& o_error_code, std::shared_ptr<SOCK>& socket, const SOCK& listener);
			bool Send(const char* data, network_error_code& o_error_code, int str_length, const std::shared_ptr<SOCK>& socket);
			bool Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket);
			bool Recieve(char* o_data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket, int str_length);
			void Reset();
			void Close_Socket(SOCK s);
			const static std::string REQUEST_ID;
#ifdef _WIN32
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_Listen_Socket;
			SOCKET m_Emergency_Socket;
#endif
			std::vector<std::thread> m_thread_pool;
			std::vector<std::shared_ptr<SOCKET>> m_socket_pool;
			threadsafe_stack<InputWrapper<InputStruct>> m_wrapper_pool;
			int m_num_client;
			int max_clients_num;
			//This is not used anymore
			Server_Phase m_Phase;
			std::mutex num_client_mutex;
			ServerLogic* m_serverlogic;
		};
	}
}