#pragma once
#include<stdio.h>
#include<string>
#include<memory>
#include "InputWrapper.h"
#include "IO_Struct.h"
#include <thread>
#include <mutex>
#ifndef SOCK
#ifdef _WIN32
#include<WinSock2.h>
#include<WS2tcpip.h>
#include"Engine/Time/Time.h"
//#pragma comment(lib,"Ws2_32.lib")
#define SOCK SOCKET
#define SOCK_INITIALIZE INVALID_SOCKET
#endif
#endif // !SOCK
namespace Network {
	static const char* GAMELOOP_PORT = "3333";
	static const char* EMERGENCY_PORT = "3334";
	enum class Client_Phase {
		REQUEST_ID,
		UPDATE_LOOP,
		INVALID,
	};
	struct network_error_code {
		std::string code;
	};
	struct ClientBuffers {
		InputStruct m_Inputs;
		InputStruct m_OtherInputs;
		InputStruct* m_input_Back = &m_Inputs;
		InputStruct* m_input_Front = &m_OtherInputs;
		bool Input_Changed = false;
		std::mutex inputs_mutex;
		UpdateStruct m_Updates;
		UpdateStruct m_OtherUpdates;
		UpdateStruct* m_update_Back = &m_Updates;
		UpdateStruct* m_update_Front = &m_OtherUpdates;
		std::mutex update_mutex;
	};
	namespace TCP {
		class Client {
		public:
			static std::shared_ptr<Client> Create_and_Run();
			static std::shared_ptr<Client> Create_and_Run(const std::string& host, const std::string& port_number);
			Client();
			void SubmitInputStruct(const InputStruct& inputs);
			UpdateStruct GetUpdateStruct();
			bool run(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Obtain_id(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Send(InputStruct* data, network_error_code& o_error_code);
			bool Recieve(UpdateStruct* data, network_error_code& o_error_code);
			void SetLogicClas(ClientLogic* logic);
			void Stop();
			float TimeSinceLastTimeStamp(uint64_t& last_time);
			void EnterningUpdatePhase();
		private:
			bool synchronize_with_Server(const std::string& host, const std::string& port_number, network_error_code& o_error_code, uint64_t& latency);
			InputStruct* InputStructure();
			UpdateStruct* UpdateStructure();
			void SwapInputStructure();
			void SwapUpdateStructure();
			bool Connect(const std::string& host, const std::string& port_number, network_error_code& o_error_code);
			bool Send(const char* data, network_error_code& o_error_code, int str_length);
			// number of bytes recieved 0 if failed
			int Recieve(std::shared_ptr<char[]>& o_data, network_error_code& o_error_code);
			bool Recieve(char* o_data, network_error_code& o_error_code,int str_length);
			void Reset();
			ClientBuffers m_data;
#ifdef _WIN32
			WSADATA m_wsaData;
			addrinfo* m_result;
			addrinfo* m_ptr;
			addrinfo m_hints;
			SOCKET m_socket;
#endif
			int m_id;
			bool flag_running;
			ClientLogic* m_client_logic;
			Client_Phase m_phase;
			float delta_time = 0.0f;
			float server_differece = 0.0f;
		};
	}
}