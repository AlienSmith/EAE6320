#include "../cEffect.h"
#include "../cShader.h"
#include "../cRenderState.h"
namespace eae6320
{
	namespace Graphics {
		class cIEffect : public cBEffect {
		public:
			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;

			eae6320::Graphics::cRenderState::Handle m_renderState;
			cIEffect():cBEffect(){}
			virtual ~cIEffect() override { CleanUp(); }
			cResult CleanUp();
			cResult InitializeShadingData();
		};
	}
}