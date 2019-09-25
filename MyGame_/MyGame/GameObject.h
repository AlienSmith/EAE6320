#pragma once
#include"Engine/Physics/sRigidBodyState.h"
#include"Engine/Assets/ReferenceCountedAssets.h"
#include"Engine/Math/sVector.h"
namespace MyGame {
	//The class that do reference count and store the trasformation
	class GameObject {
	public:
		EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
		EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(GameObject)
		static GameObject& Create(const eae6320::Math::sVector& initial_Position) {
			GameObject* gameObject = new GameObject();
			gameObject->m_rigidbodystate.position = initial_Position;
			gameObject->m_rigidbodystate.velocity = eae6320::Math::sVector(0, 0, 0);
			return *gameObject;
		}
		eae6320::Physics::sRigidBodyState m_rigidbodystate;
	private:
		GameObject() {}
		EAE6320_ASSETS_DECLAREREFERENCECOUNT()
	};
}