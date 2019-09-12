#include "cEffect.h"
namespace eae6320
{
	namespace Graphics {
		cResult cEffect::CommonCleanUp()
		{
			auto result = Results::Success;
			if (m_vertexShader)
			{
				const auto result_vertexShader = cShader::s_manager.Release(m_vertexShader);
				if (!result_vertexShader)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_vertexShader;
					}
				}
			}
			if (m_fragmentShader)
			{
				const auto result_fragmentShader = cShader::s_manager.Release(m_fragmentShader);
				if (!result_fragmentShader)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_fragmentShader;
					}
				}
			}
			if (m_renderState)
			{
				const auto result_renderState = cRenderState::s_manager.Release(m_renderState);
				if (!result_renderState)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_renderState;
					}
				}
			}
			return cResult();
		}
		cResult cEffect::commonInitializeShadingData(const sDataRequriedToIntializeEffect& data)
		{
			auto result = eae6320::Results::Success;
			if (!(result = eae6320::Graphics::cShader::s_manager.Load(data.vertex_shader_path,
				m_vertexShader, eae6320::Graphics::ShaderTypes::Vertex)))
			{
				EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
				return result;
			}
			if (!(result = eae6320::Graphics::cShader::s_manager.Load(data.fragment_shader_path,
				m_fragmentShader, eae6320::Graphics::ShaderTypes::Fragment)))
			{
				EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
				return result;
			}
			{
				constexpr uint8_t defaultRenderState = 0;
				if (!(result = eae6320::Graphics::cRenderState::s_manager.Load(defaultRenderState, m_renderState)))
				{
					EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
					return result;
				}
			}

			return result;
		}
	}
}
