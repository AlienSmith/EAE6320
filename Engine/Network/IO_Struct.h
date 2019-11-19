#pragma once
#include "Engine/Math/sVector.h"
namespace Network {
	struct InputStruct {
		int input_x_axies = 0;
		int input_y_axies = 0;
	};
	struct UpdateStruct {
		eae6320::Math::sVector position[2] = { eae6320::Math::sVector(0.0f,0.0f,0.0f),eae6320::Math::sVector(0.0f,0.0f,0.0f) };
	};
	class ClientLogic {
	public:
		InputStruct* GetInputStructure();
		UpdateStruct* GetUpdateStructure();
		void Update();
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