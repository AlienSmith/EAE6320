#include "Client.h"
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
	while (true) {
		if (Connect(host, port_number, o_error_code)) {
			if (Send(m_client_logic->GetInputStructure(), o_error_code)) {
				if (Recieve(m_client_logic->GetUpdateStructure(), o_error_code)) {
					m_client_logic->Update();
					Reset();
				}
			}
		}
	}
	return false;
}

void Network::TCP::Client::SetLogicClas(ClientLogic* logic)
{
	m_client_logic = logic;
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
Network::InputStruct* Network::TCP::Client::GetInputStructure()
{
	std::scoped_lock lock(m_data.inputs_mutex);
	InputStruct* temp;
	temp = m_data.m_input_Back;
	m_data.m_input_Back = m_data.m_input_Front;
	m_data.m_input_Front = temp;
	return m_data.m_input_Front;
}

void Network::TCP::Client::SetUpdateStruct(const UpdateStruct& data)
{
	std::scoped_lock lock(m_data.update_mutex);
	*m_data.m_update_Back = data;
	UpdateStruct* temp;
	temp = m_data.m_update_Back;
	m_data.m_update_Back = m_data.m_update_Front;
	m_data.m_update_Front = temp;
	return;
}
