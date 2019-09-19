#pragma once
#ifdef EAE6320_PLATFORM_GL
#include "OpenGL/Includes.h"
#else
#include "Direct3D/Includes.h"
#endif
#include "cVertexFormat.h"
#include "VertexFormats.h"
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
			static eae6320::cResult Create(const sDataRequriedToIntializeObject& data, DefaultGeometry* i_instance) {
				DefaultGeometry* instance = new DefaultGeometry();
				eae6320::cResult result = instance->InitializeGeometry(data);
				i_instance = instance;
				return result;
			}
		public:
			//TODO change these to private
			DefaultGeometry();
			eae6320::cResult InitializeGeometry(const sDataRequriedToIntializeObject& data);
			eae6320::cResult CleanUp();

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
		};
	}
}