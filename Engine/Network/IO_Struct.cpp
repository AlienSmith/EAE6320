#include "IO_Struct.h"
#include <iostream>
#include <thread>
#include "Engine/Time/Time.h"
//Network::InputStruct* Network::ServerLogic::GetInputStructure()
//{
//	return m_input_data;
//}

Network::UpdateStruct* Network::ServerLogic::GetUpdateStructure()
{
	return &m_update_structure;
}
void Network::ServerLogic::SetInputStructPtr(const InputStruct(*InputStruct)[MAX_CLIENT_NUMBER])
{
	m_ptr_inputs = InputStruct;
	//for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
	//	m_input_data[i] = (*InputStruct)[i];
	//}
	return;
}
//On main thread
void Network::ServerLogic::Update()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//uint64_t start = eae6320::Time::GetCurrentSystemTimeTickCount();
	//std::this_thread::sleep_for(std::chrono::milliseconds(20));
	//Movement
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	uint64_t new_time_stamp = eae6320::Time::GetCurrentSystemTimeTickCount();
	if (time_stamp != 0) {
		float delta_time = (float)eae6320::Time::ConvertTicksToSeconds(new_time_stamp - time_stamp);
		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			m_update_structure.speed[i] = eae6320::Math::sVector((float)(*m_ptr_inputs)[i].input_x_axies, (float)(*m_ptr_inputs)[i].input_y_axies, 0.0f);
			//m_update_structure.position[i] += m_update_structure.speed[i] * static_cast<float>((*m_ptr_inputs)[i].delta_time);
			m_update_structure.position[i] += m_update_structure.speed[i] * delta_time;
			m_update_structure.time_stamp = new_time_stamp;
		}
		int a = 0;
	}
	time_stamp = new_time_stamp;
	//uint64_t end = eae6320::Time::GetCurrentSystemTimeTickCount();
	//m_update_structure.update_game_delta_time = eae6320::Time::ConvertTicksToSeconds(end-start);


	//m_update_structure.speed[0] = eae6320::Math::sVector((float)(*m_ptr_inputs)[0].input_x_axies, (float)(*m_ptr_inputs)[0].input_y_axies, 0.0f);
	//m_update_structure.speed[0] *= 0.5f;
	//m_update_structure.position[0] += m_update_structure.speed[0] * 0.1f;
	//m_update_structure.speed[1] *= 0.5f;
	//m_update_structure.position[1] += m_update_structure.speed[1] * -0.2f;
	//m_update_structure.speed[1] = eae6320::Math::sVector((float)(*m_ptr_inputs)[1].input_x_axies, (float)(*m_ptr_inputs)[1].input_y_axies, 0.0f);

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

