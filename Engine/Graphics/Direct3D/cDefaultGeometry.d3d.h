#include "Includes.h"
#include "../cVertexFormat.h"
namespace eae6320 {
	namespace Graphics {
		class DefaultGeometry {
		public:
			DefaultGeometry() :m_vertexFormat(eae6320::Graphics::cVertexFormat::Handle()),
				m_vertexBuffer(nullptr) {}
			void Draw();
			eae6320::cResult CleanUp();
			eae6320::cResult InitializeGeometry();
			~DefaultGeometry();
			eae6320::Graphics::cVertexFormat::Handle m_vertexFormat;
			ID3D11Buffer* m_vertexBuffer = nullptr;
		};
	}
}