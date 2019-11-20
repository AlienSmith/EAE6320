#pragma once
#include "Engine/Math/sVector.h"
#include <time.h>
namespace Network {
	struct InputStruct {
		int input_x_axies = 0;
		int input_y_axies = 0;
	};
	struct UpdateStruct {
		eae6320::Math::sVector position[2] = { eae6320::Math::sVector(0.0f,0.0f,0.0f),eae6320::Math::sVector(0.0f,0.0f,0.0f) };
		eae6320::Math::sVector speed[2] = { eae6320::Math::sVector(0.0f,0.0f,0.0f),eae6320::Math::sVector(0.0f,0.0f,0.0f) };
		time_t last_time = 0;
	};
	class ClientLogic {
	public:
		InputStruct* GetInputStructure();
		UpdateStruct* GetUpdateStructure();
		void GenerateRandomInputs();
		void Draw();
	private:
		InputStruct m_input_Data;
		UpdateStruct m_update_structure;
	};
	class ServerLogic {
	public:
		InputStruct* GetInputStructure();
		UpdateStruct* GetUpdateStructure();
		void Update();
	private:
		InputStruct m_input_data[2];
		UpdateStruct m_update_structure;
	};
}