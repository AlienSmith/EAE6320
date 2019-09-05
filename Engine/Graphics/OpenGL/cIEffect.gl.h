#pragma once
#include "../cEffect.h"
#include "../cShader.h"
#include "../cRenderState.h"
namespace eae6320
{
	namespace Graphics {
		class cIEffect : public cEffect {
		public:
			GLuint m_programId;
			cIEffect() :cEffect(),
			m_programId(0){}
			virtual ~cIEffect() override { CleanUp(); }
			void Bind();
			cResult CleanUp();
			cResult InitializeShadingData();
		};
	}
}