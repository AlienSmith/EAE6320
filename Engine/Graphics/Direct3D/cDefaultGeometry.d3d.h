#include "Includes.h"
#include "../cVertexFormat.h"
namespace eae6320 {
	class DefaultGeometry {
	public:
		DefaultGeometry():m_vertexFormat(eae6320::Graphics::cVertexFormat::Handle()),
						m_vertexBuffer(nullptr){}
		eae6320::cResult CleanUp();
		eae6320::cResult InitializeGeometry();
		~DefaultGeometry();
	private:
		eae6320::Graphics::cVertexFormat::Handle m_vertexFormat;
		ID3D11Buffer* m_vertexBuffer = nullptr;
	};
}