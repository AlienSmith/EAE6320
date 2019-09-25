#pragma once
#ifdef EAE6320_PLATFORM_GL
#include "OpenGL/Includes.h"
#else
#include "Direct3D/Includes.h"
#endif
#include "cDefaultGeometry.h"
#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
#include "cShader.h"
#include "sContext.h"
#include "VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>
#include "cEffect.h"
#include "ConstantBufferFormats.h"
#include <cstdint>
#include "Graphics.h"
namespace eae6320 {
	namespace Graphics {
		struct sDataRequiredToRenderAFrame {
			eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;
			float clear_color[4] = {0.0f,1.0f,0.0f,1.0f};
			eae6320::Graphics::cEffect* m_Effect_Array[4];
			eae6320::Graphics::DefaultGeometry* m_Geometry_Array[4];
			int size = 0;
		};
		class View {
		public:
			View();
			~View();
			eae6320::cResult InitializeGeometry();
			eae6320::cResult InitializeShadingData();
			void SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime);
			// Submit BackGround Color
			void SubmitBackGroundColor(float r, float g, float b, float alpha);
			void SubmitEffectWithObject(cEffect* effect, DefaultGeometry* geometry);
			void SubmitCameraPerspectiveData(const sCameranPerspective& cameradata);
			eae6320::cResult WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds);
			eae6320::cResult SignalThatAllDataForAFrameHasBeenSubmitted();
			//Plateform Independent
			bool RenderFrameCheck();
			void BindAndDrawInRenderFrame();
			bool CleanSubmittedData();
			//plate-form dependent
			void RenderFrame();
			eae6320::cResult Initialize(const sInitializationParameters& i_initializationParameters);
			eae6320::cResult CleanUp();
			void CommonCleanUp();
			void SetClearColor(const float data[4]);
		private:
#ifdef EAE6320_PLATFORM_D3D
			eae6320::cResult InitializeViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
			ID3D11RenderTargetView* m_renderTargetView;
			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* m_depthStencilView;
#endif
			eae6320::Graphics::cConstantBuffer m_constantBuffer_frame;
			sDataRequiredToRenderAFrame m_dataRequiredToRenderAFrame[2];
			sDataRequiredToRenderAFrame* m_dataBeingSubmittedByApplicationThread;
			sDataRequiredToRenderAFrame* m_dataBeingRenderedByRenderThread;
			eae6320::Concurrency::cEvent m_whenAllDataHasBeenSubmittedFromApplicationThread;
			eae6320::Concurrency::cEvent m_whenDataForANewFrameCanBeSubmittedFromApplicationThread;
			float clear_color[4]{ 0.0f,0.0f,0.0f,0.0f };
		};
	}
}