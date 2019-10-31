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
#include <iostream>
#include <fstream> 
#include <chrono> 
namespace eae6320
{
	namespace Graphics {
		struct sDataRequriedToIntializeEffect {
			const char* vertex_shader_path = nullptr;
			const char* fragment_shader_path = nullptr;
		};
		class cEffect {
		private:
			cEffect() {}
			cResult CommonCleanUp();
			cResult commonInitializeShadingData(const sDataRequriedToIntializeEffect& data);
			cResult CleanUp();
			cResult InitializeShadingData(const sDataRequriedToIntializeEffect& data);
		public:
			static eae6320::cResult LoadBinary(const char* i_path, cEffect*& i_instance) {
				auto result = Results::Success;
				//Read the binary files from the address
				//auto start = std::chrono::high_resolution_clock::now();
				std::ifstream infile(i_path, std::ofstream::binary);
				//The buffer to store all the inputs Hard limite 10000 bytes 
				char* buffer = new char[1000000];
				infile.read(buffer, 1000000);
				char* vertex_shader = buffer;
				char* fragment_shader = buffer + strlen(buffer)+1;
				sDataRequriedToIntializeEffect temp;
				temp.fragment_shader_path = fragment_shader;
				temp.vertex_shader_path = vertex_shader;
				cEffect* instance = new cEffect();
				result = instance->InitializeShadingData(temp);
				i_instance = instance;
				delete[] buffer;
				return result;
			}
			static eae6320::cResult Create(const sDataRequriedToIntializeEffect& data, cEffect* & i_instance) {
				cEffect* instance = new cEffect();
				eae6320::cResult result = instance->InitializeShadingData(data);
				i_instance = instance;
				return result;
			}
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect)
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;
			eae6320::Graphics::cRenderState::Handle m_renderState;
#ifdef EAE6320_PLATFORM_GL 
			GLuint m_programId  = 0;
#endif
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
			void Bind();
			~cEffect() { CleanUp(); }
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