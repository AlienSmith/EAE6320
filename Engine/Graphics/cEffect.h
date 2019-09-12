#pragma once
#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif
#include"cShader.h"
#include"cRenderState.h"
#ifdef EAE6320_PLATFORM_GL
#include "OpenGL/Includes.h"
#elif defined EAE6320_PLATFORM_D3D
#include "Direct3D/Includes.h"
#endif
namespace eae6320
{
	namespace Graphics {
		struct sDataRequriedToIntializeEffect {
			const char* vertex_shader_path = nullptr;
			const char* fragment_shader_path = nullptr;
		};
		class cEffect {
		public:
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;
			eae6320::Graphics::cRenderState::Handle m_renderState;
#ifdef EAE6320_PLATFORM_GL 
			GLuint m_programId;
#endif
			void Bind();
			cResult CommonCleanUp();
			cResult commonInitializeShadingData();
			cResult CleanUp();
			cResult InitializeShadingData();
			cEffect() {}
			virtual ~cEffect() {}
		};
		class cBEffect {
		public:
#ifdef EAE6320_PLATFORM_GL
#else
#endif

			cBEffect() {}
			virtual ~cBEffect() {}
		};
	}
}