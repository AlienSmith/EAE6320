#pragma once
#ifdef EAE6320_PLATFORM_GL
#include "OpenGL/Includes.h"
#else
#include "Direct3D/Includes.h"
#endif
#include "cVertexFormat.h"
namespace eae6320 {
	namespace Graphics {
		class DefaultGeometry {
		public:
			DefaultGeometry();
			DefaultGeometry(const DefaultGeometry& other) = delete;
			const DefaultGeometry& operator == (const DefaultGeometry& other) = delete;
			void Draw();
			eae6320::cResult CleanUp();
			eae6320::cResult InitializeGeometry();
			~DefaultGeometry();
#ifdef EAE6320_PLATFORM_GL
			GLuint m_vertexBufferId = 0;
			GLuint m_vertexArrayId = 0;
#else
			eae6320::Graphics::cVertexFormat::Handle m_vertexFormat;
			ID3D11Buffer* m_vertexBuffer = nullptr;
#endif
		};
	}
}