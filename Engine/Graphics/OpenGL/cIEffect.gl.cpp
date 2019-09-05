#include "cIEffect.gl.h"
namespace eae6320
{
	namespace Graphics {
		void  cIEffect::Bind(){}
		cResult cIEffect::CleanUp(){
			return cEffect::CleanUp();
		}
		cResult cIEffect::InitializeShadingData(){
			return cEffect::InitializeShadingData();
		}
	}
}