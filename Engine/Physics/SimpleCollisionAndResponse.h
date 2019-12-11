#pragma once
#include "Engine/Network/IO_Struct.h"
#include "Engine/Math/sVector.h"
namespace CollisionNResponse {
	static const float radius = 1.0f;
	static const float bias = 0.0001f;
	static void Do(Network::UpdateStruct& updatestructure) {
		//Network::UpdateStruct temp = updatestructure;
		//Check once 
		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			for (int j = i+1; j < MAX_CLIENT_NUMBER; j++) {
				eae6320::Math::sVector collision_axies = updatestructure.position[i] - updatestructure.position[j];
				float temp_length = collision_axies.GetLength();
				//Collided
				if (temp_length < (radius *2.0f -bias)) {
					float difference = radius * 2.0f - temp_length +bias;
					difference /= 2.0f;
					collision_axies /= temp_length;
					updatestructure.position[i] += collision_axies * difference;
					updatestructure.position[j] -= collision_axies * difference;
					float speed_on_aixes = eae6320::Math::Dot((updatestructure.speed[i] - updatestructure.speed[j]),collision_axies);
					speed_on_aixes /= 2.0f;
					speed_on_aixes *= 2.5f;
					updatestructure.speed[i] -= speed_on_aixes * collision_axies;
					updatestructure.speed[j] += speed_on_aixes * collision_axies;
				}
			}
		}
		//Check twice and delete objects that collided with two other objects
	}
	static void OutofRangeReset(Network::UpdateStruct& updatestructure) {
		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			if (abs(updatestructure.position[i].x) > 9.0f || abs(updatestructure.position[i].y) > 9.0f) {
				updatestructure.position[i] = eae6320::Math::sVector(0.0f,0.0f,-10.0f);
			}
		}
	}
}