#include "IO_Struct.h"
#include <iostream>
Network::InputStruct* Network::ServerLogic::GetInputStructure()
{
	return m_input_data;
}

Network::UpdateStruct* Network::ServerLogic::GetUpdateStructure()
{
	return &m_update_structure;
}
//Run the logic of the updates
void Network::ServerLogic::Update()
{
	m_input_data[0].input_x_axies = 1;
	m_input_data[1].input_x_axies = 2;
	m_update_structure.position[0] += eae6320::Math::sVector((float)m_input_data[0].input_x_axies, (float)m_input_data[0].input_y_axies, 0.0f);
	m_update_structure.position[1] += eae6320::Math::sVector((float)m_input_data[1].input_x_axies, (float)m_input_data[1].input_y_axies, 0.0f);
	return;
}

Network::InputStruct* Network::ClientLogic::GetInputStructure()
{
	return &m_input_Data;
}

Network::UpdateStruct* Network::ClientLogic::GetUpdateStructure()
{
	return &m_update_structure;
}

void Network::ClientLogic::Update()
{
	//inputs
	{
		std::cout << "Input X-axies" << std::endl;
		std::cin >> m_input_Data.input_x_axies;
		std::cout << "Input Y-axies" << std::endl;
		std::cin >> m_input_Data.input_y_axies; 
	}
	//Draws
	{
		std::cout << "Player one at( ";
		std::cout << m_update_structure.position[0].x<< ",";
		std::cout << m_update_structure.position[0].y<< ",";
		std::cout << m_update_structure.position[0].z<< ")";
		std::cout << std::endl;

		std::cout << "Player two at( ";
		std::cout << m_update_structure.position[1].x << ",";
		std::cout << m_update_structure.position[1].y << ",";
		std::cout << m_update_structure.position[1].z << ")";
		std::cout << std::endl;
	}
	return;
}
