#include "../cEffect.h"
#include "../cShader.h"
#include "../cRenderState.h"
namespace eae6320
{
	namespace Graphics {
		class cIEffect : public cEffect {
		public:
			cIEffect():cEffect(){}
			virtual ~cIEffect() override { CleanUp(); }
			void Bind();
			cResult CleanUp();
			cResult InitializeShadingData();
		};
	}
}