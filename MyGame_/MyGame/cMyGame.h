/*
	This class is your specific game
*/
#ifndef EAE6320_CMyGame_H
#define EAE6320_CMyGame_H

// Includes
//=========
///this have to be the first include
#include"Engine/Physics/sRigidBodyState.h"
#include <Engine/Application/cbApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cDefaultGeometry.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/3DAudio/3DAudio.h>
#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif
#include <Engine/Network/Client.h>

// Class Declaration
//==================

namespace eae6320
{
	struct RenderAbleObject {
		eae6320::Physics::sRigidBodyState* m_ridgebody;
		Graphics::cEffect* m_effect;
		Graphics::DefaultGeometry* m_Geometry;
	};
	class cExampleGame : public Application::cbApplication
	{
		// Inherited Implementation
		//=========================

	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		virtual const char* GetMainWindowName() const override
		{
			return "SuperMassiveBlackHole"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
				;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		virtual const char* GetMainWindowClassName() const override { return "SuperMassiveBlackHole"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		virtual const WORD* GetLargeIconId() const override { static constexpr WORD iconId_large = IDI_EAEALIEN; return &iconId_large; }
		virtual const WORD* GetSmallIconId() const override { static constexpr WORD iconId_small = IDI_EAEALIEN; return &iconId_small; }
		virtual void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate);
		virtual void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate);
#endif

		// Run
		//----

		virtual void UpdateBasedOnInput() override;

		// Initialization / Clean Up
		//--------------------------

		virtual cResult Initialize() override;
		virtual cResult CleanUp() override;
		Graphics::cEffect* m_white_Effect = nullptr;
		Graphics::cEffect* m_flash_Effect = nullptr;
		Graphics::DefaultGeometry* m_quard = nullptr;
		Graphics::DefaultGeometry* m_triangle = nullptr;
		eae6320::Physics::sRigidBodyState m_camera;
		eae6320::Physics::sRigidBodyState m_object[MAX_CLIENT_NUMBER];
		eae6320::Physics::sRigidBodyState m_Plane;
		Assets::cHandle<Graphics::DefaultGeometry> m_triangle_handle;
		Assets::cHandle<Graphics::DefaultGeometry> m_cube_handle;
		Assets::cHandle<Graphics::DefaultGeometry> m_sphere_handle;
		Assets::cHandle<Graphics::DefaultGeometry> m_plane_handle;

		bool m_showotherobject = false;
		bool m_changeeffect = false;

		std::shared_ptr<Network::TCP::Client> m_client;
		Network::InputStruct m_input_struct;
		Network::UpdateStruct m_update_struct;
		eae6320::Audio3D::AudioSource* MySource = nullptr;
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, System::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CMyGame_H
