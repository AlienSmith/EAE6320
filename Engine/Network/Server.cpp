#include "Server.h"
#include <assert.h>
bool Network::TCP::Server::Run(const std::string& port_number, network_error_code& o_error_code)
{
	char* recv_data = nullptr;
	//Start the timer
	{
		eae6320::Time::Initialize();
	}
	//Distribute Id on the emergency thread
	{
		//Interestingly the start function have to be called within the main thread 
		//Assume it invoke functions that are not thread safe
		if (Start(EMERGENCY_PORT, o_error_code, m_Emergency_Socket)) {
			std::thread Emergency_Thread{
					[this]() {
					bool iResult = true;
					network_error_code emergency_error_code;
					while (true) {
						std::shared_ptr<SOCK> temp(new SOCK());
						*temp = SOCK_INITIALIZE;
						Sleep(1);
						if (Accept(emergency_error_code, temp, m_Emergency_Socket)) {
							std::thread temp_thread{ [this,temp]() {
								Network::network_error_code error_code;
								if (!IntepretRequest(error_code, temp)) {
									printf(error_code.code.c_str());
								}
								return;
							} };
							temp_thread.detach();
						}
						else {
							iResult = false;
						}
					}
					if (!iResult) {
						printf(emergency_error_code.code.c_str());
					}
					return;
					}
			};
			Emergency_Thread.detach();
		}
	}
	// Recieve and send data on the listen socket
	if (!Start(GAMELOOP_PORT, o_error_code, m_Listen_Socket)) {
		return false;
	}
	//Launch thread to gather inputs from clients
	{
		std::thread Inputs_Gathering_Thread{ [this]() {
			while (true) {
				network_error_code gather_error;
				std::shared_ptr<SOCK> temp_socket(new SOCK());
				*temp_socket = SOCK_INITIALIZE;
				Sleep(1);
				if (Accept(gather_error, temp_socket, m_Listen_Socket)) {
					std::thread temp_thread{ [this,temp_socket]() {
						Network::network_error_code error_code;
						Network::InputWrapper<InputStruct> temp_input;
						if (Recieve(&temp_input, error_code, temp_socket)) {
							SubmitInputrecord(temp_input, temp_socket);
						}
						else {
							printf(error_code.code.c_str());
						}
						return;
					} };
					temp_thread.detach();
				}
			}
		} };
		Inputs_Gathering_Thread.detach();
	}
	//Run the game loop on the main thread
	{
		while (true) {
			if (SwapInputBuffers()) {
				UpdateServerLogic();
				Sleep(20);
				//Send the updated result to all the Connected sockets
				for (auto it = begin(*m_buffer.Ptr_socket_front); it != end(*m_buffer.Ptr_socket_front); it++) {
					std::shared_ptr<SOCK> temp_socket = *it;
					std::thread temp_socket_thread{ [this,temp_socket]() {
						network_error_code send_error;
						if (!Send(reinterpret_cast<char*>(m_serverlogic->GetUpdateStructure()), send_error, sizeof(*(m_serverlogic->GetUpdateStructure())), temp_socket)) {
							printf(send_error.code.c_str());
						}
						Close_Socket(*(temp_socket));
						return;
					} };
					m_thread_pool.push_back(std::move(temp_socket_thread));
				}
				//Clear the front buffer after sending are done
				for (auto it = begin(m_thread_pool); it != end(m_thread_pool); it++) {
					if (it->joinable()) {
						it->join();
					}
				}
				m_thread_pool.clear();
				m_buffer.Ptr_socket_front->clear();
			}
		}
	}
	//Destroied the timer
	{
		eae6320::Time::CleanUp();
	}
	//{
	//	//Use the main thread to run Game Loop
	//	//TODO change this to work like the client sides
	//	if (Start(GAMELOOP_PORT, o_error_code, m_Listen_Socket)) {
	//		while (true) {
	//			for (int i = 0; i < max_clients_num; i++) {
	//				std::shared_ptr<SOCK> temp_socket(new SOCK());
	//				*temp_socket = SOCK_INITIALIZE; 
	//				Sleep(1);
	//				if (Accept(o_error_code, temp_socket, m_Listen_Socket)) {
	//					m_socket_pool.push_back(temp_socket);
	//					std::thread temp_thread{ [this,temp_socket]() {
	//						Network::network_error_code error_code;
	//						Network::InputWrapper<InputStruct> temp_input;
	//						if (Recieve(&temp_input, error_code, temp_socket)) {
	//							m_wrapper_pool.push(temp_input);
	//						}
	//						else {
	//							printf(error_code.code.c_str());
	//						}
	//					} };
	//					//in main thread
	//					m_thread_pool.push_back(std::move(temp_thread));
	//				}
	//			}
	//			for (auto it = begin(m_thread_pool); it != end(m_thread_pool); it++) {
	//				if (it->joinable()) {
	//					it->join();
	//				}
	//			}
	//			m_thread_pool.clear();
	//			//Put Inputs to ServerLogic class
	//			for (int i = 0; i < m_num_client; i++) {
	//				Network::InputWrapper<InputStruct> temp_input;
	//				m_wrapper_pool.pop(temp_input);
	//				(m_serverlogic->GetInputStructure())[temp_input.Socket_id - 1] = temp_input.t;
	//			}		
	//			m_serverlogic->Update();
	//			//This is not multithreaded Send the updated result to all clients
	//			for (auto it = begin(m_socket_pool); it != end(m_socket_pool); it++) {
	//				Send(reinterpret_cast<char*>(m_serverlogic->GetUpdateStructure()), o_error_code, sizeof(*(m_serverlogic->GetUpdateStructure())), *it);
	//				Close_Socket(*(it->get()));
	//			}
	//			m_socket_pool.clear();
	//		}
	//	}
	//}
	return true;
}
void Network::TCP::Server::SubmitInputrecord(const InputWrapper<InputStruct>& o_data, std::shared_ptr<SOCK> Socket)
{
	std::scoped_lock lock(m_buffer.inputsmutex);
	assert(m_buffer.ptr_back);
	printf("Recieve inputs form %d", o_data.Socket_id);
	m_buffer.need_swap = true;
	(*(m_buffer.ptr_back))[o_data.Socket_id-1] = o_data.t;
	(m_buffer.Ptr_socket_back)->push_back(Socket);
	return;
}
bool Network::TCP::Server::SwapInputBuffers()
{
	std::scoped_lock lock(m_buffer.inputsmutex);
	if (m_buffer.need_swap) {
		InputStruct(*ptr_temp)[MAX_CLIENT_NUMBER];
		ptr_temp = m_buffer.ptr_back;
		m_buffer.ptr_back = m_buffer.ptr_front;
		m_buffer.ptr_front = ptr_temp;
		std::vector<std::shared_ptr<SOCK>>* ptr_sockets_temp;
		ptr_sockets_temp = m_buffer.Ptr_socket_back;
		m_buffer.Ptr_socket_back = m_buffer.Ptr_socket_front;
		m_buffer.Ptr_socket_front = ptr_sockets_temp;
		m_buffer.need_swap = false;
		return true;
	}
	return false;
}
void Network::TCP::Server::UpdateServerLogic()
{
	//Copy the input buffer to server and do update on it.
	m_serverlogic->SetInputStructPtr(m_buffer.ptr_front);
	m_serverlogic->Update();
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
		else {
			uint64_t time_stamp = eae6320::Time::GetCurrentSystemTimeTickCount();
			char* send = reinterpret_cast<char*>(&time_stamp);
			int buf_length = sizeof(time_stamp);
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