#include "Client.h"
#include <time.h>
std::shared_ptr<Network::TCP::Client> Network::TCP::Client::Create_and_Run(const std::string& host, const std::string& port_number)
{
	std::shared_ptr<Network::TCP::Client> client(new Client());
	std::thread temp{ [client,host,port_number]() {
		Network::network_error_code error_code;
		if (!client->run(host, port_number, error_code)) {
			printf("Error \n");
			printf(error_code.code.c_str());
		}
	} };
	temp.detach();
	return client;
}
bool Network::TCP::Client::Obtain_id(const std::string& host, const std::string& port_number, network_error_code& o_error_code)
{
	std::shared_ptr<char[]> temp;
	if (Connect(host, port_number, o_error_code)) {
		if (Send("REQUEST_ID", o_error_code, ((int)strlen("REQUEST_ID")) + 1)) {
			if (Recieve(temp, o_error_code) != -1) {
				m_id = *(reinterpret_cast<int*> (temp.get()));
				printf("My id is %d", m_id);
				return true;
			}
		}
	}
	return false;
}

bool Network::TCP::Client::Send(InputStruct* data, network_error_code& o_error_code)
{
	Network::InputWrapper<InputStruct> temp;
	temp.t = *data;
	temp.Socket_id = m_id;
	return Send(reinterpret_cast<char*>(&temp), o_error_code, sizeof(temp));
}

bool Network::TCP::Client::Recieve(UpdateStruct* data, network_error_code& o_error_code)
{
	return Recieve(reinterpret_cast<char*>(data), o_error_code, sizeof(*data));
}

bool Network::TCP::Client::run(const std::string& host, const std::string& port_number, network_error_code& o_error_code)
{
	//Obtain ID
	{
		if (!Obtain_id(host, port_number, o_error_code)) {
			return false;
		}
		Reset();
	}
	Sleep(5000);
	while (flag_running) {
		while (flag_running && m_phase == Client_Phase::UPDATE_LOOP) {
			if (Connect(host, port_number, o_error_code)) {
				if (Send(InputStructure(), o_error_code)) {
					if (Recieve(UpdateStructure(), o_error_code)) {
						SwapUpdateStructure();
						Reset();
					}
				}
			}
		}
	}
	Reset();
	return false;
}

void Network::TCP::Client::SetLogicClas(ClientLogic* logic)
{
	m_client_logic = logic;
}

void Network::TCP::Client::Stop()
{
	flag_running = false;
}

float Network::TCP::Client::TimeSinceLastTimeStamp(time_t& last_time)
{
	time_t current_time = time(NULL);
	return (float)difftime(current_time, last_time);
}

void Network::TCP::Client::EnterningUpdatePhase()
{
	if (m_phase == Client_Phase::REQUEST_ID) {
		m_phase = Client_Phase::UPDATE_LOOP;
	}
}

void Network::TCP::Client::SubmitInputStruct(const Network::InputStruct& inputs) {
	std::scoped_lock lock(m_data.inputs_mutex);
	*m_data.m_input_Back = inputs;
}
Network::UpdateStruct Network::TCP::Client::GetUpdateStruct()
{
	std::scoped_lock lock(m_data.update_mutex);
	return *(m_data.m_update_Front);
}
Network::InputStruct* Network::TCP::Client::InputStructure()
{
	SwapInputStructure();
	return m_data.m_input_Front;
}

Network::UpdateStruct* Network::TCP::Client::UpdateStructure()
{
	return m_data.m_update_Back;
}

void Network::TCP::Client::SwapInputStructure()
{
	std::scoped_lock lock(m_data.inputs_mutex);
	InputStruct* temp;
	temp = m_data.m_input_Back;
	m_data.m_input_Back = m_data.m_input_Front;
	m_data.m_input_Front = temp;
	return;
}

void Network::TCP::Client::SwapUpdateStructure()
{
	std::scoped_lock lock(m_data.update_mutex);
	UpdateStruct* temp;
	temp = m_data.m_update_Back;
	m_data.m_update_Back = m_data.m_update_Front;
	m_data.m_update_Front = temp;
}


