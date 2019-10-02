#pragma once
#ifdef EAE6320_PLATFORM_GL
#include "OpenGL/Includes.h"
#else
#include "Direct3D/Includes.h"
#endif
#include <Engine/Assets/cHandle.h>
#include <Engine/Assets/cManager.h>
#include "cVertexFormat.h"
#include "VertexFormats.h"
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
namespace eae6320 {
	namespace Graphics {
		struct sDataRequriedToIntializeObject {
			int vertexcount = 0;
			eae6320::Graphics::VertexFormats::s3dObject * vertexData = nullptr;
			int indexcount = 0;
			uint16_t * indexdata = nullptr;
		};
		class DefaultGeometry {
		public:
			static Assets::cManager<DefaultGeometry> s_manager;
			static cResult Load(const std::string& i_path, DefaultGeometry*& o_instance) {
				auto result = Results::Success;
				Platform::sDataFromFile dataFromFile;
				DefaultGeometry* newGeometry = nullptr;
				cScopeGuard scopeGuard([&o_instance, &result, &dataFromFile, &newGeometry]
					{
						if (result)
						{
							EAE6320_ASSERT(newShader != nullptr);
							o_instance = newGeometry;
						}
						else
						{
							if (newGeometry)
							{
								newGeometry->DecrementReferenceCount();
								newGeometry = nullptr;
							}
							o_instance = nullptr;
						}
					});
				// Load the binary data
				{
					std::string errorMessage;
					if (!(result = Platform::LoadBinaryFile(i_path.c_str(), dataFromFile, &errorMessage)))
					{
						EAE6320_ASSERTF(false, errorMessage.c_str());
						Logging::OutputError("Failed to load shader from file %s: %s", i_path.c_str(), errorMessage.c_str());
						return result;
					}
				}
				// Allocate new geoemtry
				{
					newGeometry = new (std::nothrow) DefaultGeometry();
					if (!newGeometry)
					{
						result = Results::OutOfMemory;
						EAE6320_ASSERTF(false, "Couldn't allocate memory for the shader %s", i_path.c_str());
						Logging::OutputError("Failed to allocate memory for the shader %s", i_path.c_str());
						return result;
					}
				}
				/*if (!(result = ->Initialize(i_path, dataFromFile)))
				{
					EAE6320_ASSERTF(false, "Initialization of new shader failed");
					return result;
				}*/
				return result;
			}
			static eae6320::cResult Create(const sDataRequriedToIntializeObject& data, DefaultGeometry* & i_instance) {
				DefaultGeometry* instance = new DefaultGeometry();
				eae6320::cResult result = instance->InitializeGeometry(data);
				i_instance = instance;
				return result;
			}
			//TODO change these to private

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(DefaultGeometry)
			const DefaultGeometry& operator == (const DefaultGeometry& other) = delete;
			void Draw();
			~DefaultGeometry();
			//Add indexCountToRender and indexbuffer
			int indexCountToRender = 6;
#ifdef EAE6320_PLATFORM_GL
			GLuint m_vertexBufferId = 0;
			GLuint m_vertexArrayId = 0;
			GLuint m_indexBufferId = 0;
#elif defined EAE6320_PLATFORM_D3D
			//The reference count was put at the last place to make the structure smaller
			eae6320::Graphics::cVertexFormat::Handle m_vertexFormat;
			ID3D11Buffer* m_vertexBuffer = nullptr;
			ID3D11Buffer* m_indexBuffer = nullptr;
#endif
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()
		private:
			DefaultGeometry();
			eae6320::cResult InitializeGeometry(const sDataRequriedToIntializeObject& data);
			eae6320::cResult CleanUp();
		};
	}
}