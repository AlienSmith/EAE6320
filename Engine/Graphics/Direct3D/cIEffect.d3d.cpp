#include "../cEffect.h"
#include "../Graphics.h"
#include "Includes.h"
#include "../cConstantBuffer.h"
#include "../ConstantBufferFormats.h"
#include "../cRenderState.h"
#include "../cShader.h"
#include "../cVertexFormat.h"
#include "../sContext.h"
#include "../VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>
namespace eae6320
{
	namespace Graphics {
		void cEffect::Bind()
		{
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);
			{
				constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
				constexpr unsigned int interfaceCount = 0;
				// Vertex shader
				{
					EAE6320_ASSERT(m_vertexShader);
					auto* const shader = cShader::s_manager.Get(m_vertexShader);
					EAE6320_ASSERT(shader && shader->m_shaderObject.vertex);
					direct3dImmediateContext->VSSetShader(shader->m_shaderObject.vertex, noInterfaces, interfaceCount);
				}
				// Fragment shader
				{
					EAE6320_ASSERT(m_fragmentShader);
					auto* const shader = cShader::s_manager.Get(m_fragmentShader);
					EAE6320_ASSERT(shader && shader->m_shaderObject.fragment);
					direct3dImmediateContext->PSSetShader(shader->m_shaderObject.fragment, noInterfaces, interfaceCount);
				}
			}
			// Render state
			{
				EAE6320_ASSERT(m_renderState);
				auto* const renderState = cRenderState::s_manager.Get(m_renderState);
				EAE6320_ASSERT(renderState);
				renderState->Bind();
			}
			return;
		}
		cResult eae6320::Graphics::cEffect::CleanUp()
		{
			return cEffect::CommonCleanUp();
		}

		cResult eae6320::Graphics::cEffect::InitializeShadingData()
		{
			return cEffect::commonInitializeShadingData();
		}
	}
}
