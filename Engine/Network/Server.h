#pragma once
#include "Client.h"
#include <mutex>
#include <vector>
#include "stack.h"
#include "InputWrapper.h"
namespace Network {
	namespace TCP {
		enum class Server_Phase {
			DISTRIBUTE_ID,
			GAMELOOP_UPDATE,
			INVALID,
		};
		struct ServerInputBuffers {
			InputStruct front_buffer[MAX_CLIENT_NUMBER];
			InputStruct back_buffer[MAX_CLIENT_NUMBER];
			InputStruct(*ptr_front)[MAX_CLIENT_NUMBER] = &front_buffer;
			InputStruct(*ptr_back)[MAX_CLIENT_NUMBER] = &back_buffer;
			std::vector<std::shared_ptr<SOCK>> Socket_pool_front;
			std::vector<std::shared_ptr<SOCK>> Socket_pool_back;
			std::vector<std::shared_ptr<SOCK>>* Ptr_socket_back = & Socket_pool_back;
			std::vector<std::shared_ptr<SOCK>>* Ptr_socket_front = &Socket_pool_front;
			bool need_swap = false;
			std::mutex inputsmutex;
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
			//I have not implement the move assignment operator 
			void SubmitInputrecord(const InputWrapper<InputStruct>& o_data, std::shared_ptr<SOCK> Socket);
			bool SwapInputBuffers();
			void UpdateServerLogic();
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
			ServerInputBuffers m_buffer;
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