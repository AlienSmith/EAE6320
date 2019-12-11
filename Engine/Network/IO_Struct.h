#pragma once
#include "Engine/Math/sVector.h"
#include <time.h>
#include <stdint.h>
#define MAX_CLIENT_NUMBER 2
namespace Network {
	struct InputStruct {
		int input_x_axies = 0;
		int input_y_axies = 0;
		float delta_time = 0;
	};
	struct UpdateStruct {
		eae6320::Math::sVector position[MAX_CLIENT_NUMBER];
		eae6320::Math::sVector speed[MAX_CLIENT_NUMBER];
		eae6320::Math::sVector acceleration[MAX_CLIENT_NUMBER];
		float time_stamp = 0.0f;
		UpdateStruct() {
			for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
				position[i] = eae6320::Math::sVector((float)i, 0.0f, -10.0f);
				speed[i] = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
				acceleration[i] = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
			}
		}
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
		//InputStruct* GetInputStructure();
		UpdateStruct* GetUpdateStructure();
		void SetInputStructPtr(const InputStruct(*InputStruct)[MAX_CLIENT_NUMBER]);
		void Update();
	private:
		//InputStruct m_input_data[MAX_CLIENT_NUMBER];
		//const InputStruct(*m_ptr_inputs)[MAX_CLIENT_NUMBER] = &m_input_data;
		const InputStruct(*m_ptr_inputs)[MAX_CLIENT_NUMBER] = nullptr;
		UpdateStruct m_update_structure;
		uint64_t time_stamp = 0;
	};
}