#pragma once
#include "Server.h"

namespace Network {
	namespace TCP {
		enum class ServerPhase {
			DISTRIBUTE_ID,
			GAMELOOP_UPDATE,
			INVALID,
		};
		template<class Inputstructure,class UpdateStructure>
		class ServerProxy {
		public:
			ServerProxy() {}
			bool Run(const std::string& port_number, network_error_code& o_error_code) {
				char* recv_data = nullptr;
				if (Start(port_number, o_error_code)) {
					//Distribute Id Phase
					if (m_Phase == Server_Phase::DISTRIBUTE_ID)
					{
						for (int i = 0; i < max_clients_num; i++) {
							std::shared_ptr<SOCK> temp(new SOCK());
							*temp = SOCK_INITIALIZE;
							Sleep(1);
							if (m_server.Accept(o_error_code, temp)) {
								std::thread temp_thread{ [this,temp]() {
									Network::network_error_code error_code;
									if (!IntepretRequest(error_code, temp)) {
										printf(error_code.code.c_str());
									}
								} };
								m_thread_pool.push_back(std::move(temp_thread));
							}
						}
						for (auto it = begin(m_thread_pool); it != end(m_thread_pool); it++) {
							it->join();
						}
						printf("all ids distributed\n");
						m_Phase = Server_Phase::GAMELOOP_UPDATE;
					}
					//Game Loop
					while (true) {
						for (int i = 0; i < max_clients_num; i++) {
							std::shared_ptr<SOCK> temp_socket(new SOCK());
							*temp_socket = INVALID_SOCKET;
							Sleep(1);
							if (m_server.Accept(o_error_code, temp_socket)) {
								m_socket_pool.push_back(temp_socket);
								std::thread temp_thread{ [this,temp_socket]() {
									Network::network_error_code error_code;
									Network::InputWrapper<Inputstructure> temp_input;
									if (Recieve(&temp_input, error_code, temp_socket)) {
										m_wrapper_pool.push(temp_input);
									}
									else {
										printf(error_code.code.c_str());
									}
								} };
								//in main thread
								m_thread_pool.push_back(std::move(temp_thread));
							}
						}
						for (auto it = begin(m_thread_pool); it != end(m_thread_pool); it++) {
							if (it->joinable()) {
								it->join();
							}
						}
						m_thread_pool.clear();
						//Put Inputs to ServerLogic class
						for (int i = 0; i < m_num_client; i++) {
							Network::InputWrapper<Inputstructure> temp_input;
							m_wrapper_pool.pop(temp_input);
							(m_serverlogic->GetInputStructure())[temp_input.Socket_id - 1] = temp_input.t;
						}
						m_serverlogic->Update();
						//This is not multithreaded Send the updated result to all clients
						//Write A close socket function
						for (auto it = begin(m_socket_pool); it != end(m_socket_pool); it++) {
							Send(m_serverlogic->GetUpdateStructure(), o_error_code, *it);
							closesocket(*(it->get()));
						}
						m_socket_pool.clear();
					}
				}
				return false;
			}
			bool Send(UpdateStructure* data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket) {
				return m_server.Send(reinterpret_cast<char*> data, o_error_code,sizeof(*data),socket);
			}
			bool IntepretRequest(network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket) {
				std::shared_ptr<char[]> recv_data = nullptr;
				if (m_server.Recieve(recv_data, o_error_code, socket)) {
					std::string recived(recv_data.get());
					if (recived == REQUEST_ID) {
						m_num_client++;
						char* send = reinterpret_cast<char*>(&m_num_client);
						int buf_length = sizeof(m_num_client);
						if (m_server.Send(send, o_error_code, buf_length, socket)) {
							return true;
						}
					}
				}
				return false;
			}
			void SetServerLogic(ServerLogic* serverlogic) {
				m_serverlogic = serverlogic;
			}
			bool Recieve(InputWrapper<Inputstructure>* o_data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket) {
				return m_server.Recieve(reinterpret_cast<char*>(o_data),o_error_code,socket,sizeof(*o_data))；
			}
		private:
			Server m_server;
			std::vector<std::thread> m_thread_pool;
			std::vector<std::shared_ptr<SOCK>> m_socket_pool;
			threadsafe_stack<InputWrapper<Inputstructure>> m_wrapper_pool;

			int m_num_client;
			int max_clients_num;
			Server_Phase m_Phase;
			std::mutex num_client_mutex;
			ServerLogic* m_serverlogic;
		};
	}
}