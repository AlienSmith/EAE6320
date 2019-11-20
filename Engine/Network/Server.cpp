#include "Server.h"
bool Network::TCP::Server::Run(const std::string& port_number, network_error_code& o_error_code)
{
	char* recv_data = nullptr;
	if (Start(port_number, o_error_code)) {
		//Distribute Id Phase
		if (m_Phase == Server_Phase::DISTRIBUTE_ID)
		{
			for (int i = 0; i < max_clients_num; i++) {
				std::shared_ptr<SOCK> temp(new SOCK());
				*temp = SOCK_INITIALIZE;
				Sleep(1);
				if (Accept(o_error_code, temp)) {
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
				*temp_socket = SOCK_INITIALIZE;
				Sleep(1);
				if (Accept(o_error_code, temp_socket)) {
					m_socket_pool.push_back(temp_socket);
					std::thread temp_thread{ [this,temp_socket]() {
						Network::network_error_code error_code;
						Network::InputWrapper<InputStruct> temp_input;
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
				Network::InputWrapper<InputStruct> temp_input;
				m_wrapper_pool.pop(temp_input);
				(m_serverlogic->GetInputStructure())[temp_input.Socket_id - 1] = temp_input.t;
			}
			m_serverlogic->Update();
			//This is not multithreaded Send the updated result to all clients
			for (auto it = begin(m_socket_pool); it != end(m_socket_pool); it++) {
				Send(reinterpret_cast<char*>(m_serverlogic->GetUpdateStructure()), o_error_code, sizeof(*(m_serverlogic->GetUpdateStructure())), *it);
				Close_Socket(*(it->get()));
			}
			m_socket_pool.clear();
		}
	}
	return false;
}

bool Network::TCP::Server::Send(UpdateStruct* data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket)
{
	return Send(reinterpret_cast<char*>(data), o_error_code, sizeof(*data), socket);
}

bool Network::TCP::Server::Recieve(InputWrapper<InputStruct>* o_data, network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket)
{
	return Recieve(reinterpret_cast<char*>(o_data), o_error_code, socket, sizeof(*o_data));
}

bool Network::TCP::Server::IntepretRequest(network_error_code& o_error_code, const std::shared_ptr<SOCK>& socket)
{
	std::shared_ptr<char[]> recv_data = nullptr;
	if (Recieve(recv_data, o_error_code, socket)) {
		std::string recived(recv_data.get());
		if (recived == REQUEST_ID) {
			m_num_client++;
			char* send = reinterpret_cast<char*>(&m_num_client);
			int buf_length = sizeof(m_num_client);
			if (Send(send, o_error_code, buf_length, socket)) {
				return true;
			}
		}
	}
	return false;
}

void Network::TCP::Server::SetServerLogic(ServerLogic* serverlogic)
{
	m_serverlogic = serverlogic;
}