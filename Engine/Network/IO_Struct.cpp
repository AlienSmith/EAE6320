#include "IO_Struct.h"
#include <iostream>
#include <thread>
Network::InputStruct* Network::ServerLogic::GetInputStructure()
{
	return m_input_data;
}

Network::UpdateStruct* Network::ServerLogic::GetUpdateStructure()
{
	return &m_update_structure;
}
void Network::ServerLogic::Update()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	time_t current_time = time(NULL);
	float delta_time;
	if (m_update_structure.last_time == 0) {
		delta_time = 0;
	}
	else {
		delta_time = (float)difftime(current_time, m_update_structure.last_time);
	}
	m_update_structure.speed[0] = eae6320::Math::sVector((float)m_input_data[0].input_x_axies, (float)m_input_data[0].input_y_axies, 0.0f);
	m_update_structure.speed[1] = eae6320::Math::sVector((float)m_input_data[1].input_x_axies * -1, (float)m_input_data[1].input_y_axies*-1, 0.0f);
	m_update_structure.speed[0] *= 0.01f;
	m_update_structure.speed[1] *= 0.01f;
	m_update_structure.position[0] += m_update_structure.speed[0] * delta_time;
	m_update_structure.position[1] += m_update_structure.speed[1] * delta_time;
	m_update_structure.last_time = current_time;
	return ;
}
//Run the logic of the updates
//time_t Network::ServerLogic::Update()
//{
//	m_update_structure.position[0] += eae6320::Math::sVector((float)m_input_data[0].input_x_axies, (float)m_input_data[0].input_y_axies, 0.0f);
//	m_update_structure.position[1] += eae6320::Math::sVector((float)m_input_data[1].input_x_axies, (float)m_input_data[1].input_y_axies, 0.0f);
//	return;
//}

Network::InputStruct* Network::ClientLogic::GetInputStructure()
{
	return &m_input_Data;
}

Network::UpdateStruct* Network::ClientLogic::GetUpdateStructure()
{
	return &m_update_structure;
}

void Network::ClientLogic::GenerateRandomInputs()
{
	m_input_Data.input_x_axies = 0;
	m_input_Data.input_y_axies = 1;
}

void Network::ClientLogic::Draw()
{
	std::cout << "Player one at( ";
	std::cout << m_update_structure.position[0].x << ",";
	std::cout << m_update_structure.position[0].y << ",";
	std::cout << m_update_structure.position[0].z << ")";
	std::cout << std::endl;

	std::cout << "Player two at( ";
	std::cout << m_update_structure.position[1].x << ",";
	std::cout << m_update_structure.position[1].y << ",";
	std::cout << m_update_structure.position[1].z << ")";
	std::cout << std::endl;
}

