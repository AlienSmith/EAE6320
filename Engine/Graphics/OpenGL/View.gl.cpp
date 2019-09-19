#include "../View.h"
namespace eae6320 {
	namespace Graphics {
		View::View() :m_constantBuffer_frame(eae6320::Graphics::cConstantBuffer(eae6320::Graphics::ConstantBufferTypes::Frame)),
			m_dataRequiredToRenderAFrame{ sDataRequiredToRenderAFrame(),sDataRequiredToRenderAFrame() },
			m_dataBeingSubmittedByApplicationThread(&m_dataRequiredToRenderAFrame[0]),
			m_dataBeingRenderedByRenderThread(&m_dataRequiredToRenderAFrame[1]),
			m_whenAllDataHasBeenSubmittedFromApplicationThread(eae6320::Concurrency::cEvent()),
			m_whenDataForANewFrameCanBeSubmittedFromApplicationThread(eae6320::Concurrency::cEvent()){}
		void View::RenderFrame()
		{
			if (!RenderFrameCheck()) {
				return;
			}
			// Every frame an entirely new image will be created.
			// Before drawing anything, then, the previous image will be erased
			// by "clearing" the image buffer (filling it with a solid color)
			{
				// Black is usually used
				{
					glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				{
					constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
					glClear(clearColor);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
			}
			// In addition to the color buffer there is also a hidden image called the "depth buffer"
			// which is used to make it less important which order draw calls are made.
			// It must also be "cleared" every frame just like the visible color buffer.
			{
				{
					glDepthMask(GL_TRUE);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
					constexpr GLclampd clearToFarDepth = 1.0;
					glClearDepth(clearToFarDepth);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				{
					constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
					glClear(clearDepth);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
			}

			EAE6320_ASSERT(m_dataBeingRenderedByRenderThread);

			// Update the frame constant buffer
			{
				// Copy the data from the system memory that the application owns to GPU memory
				auto& constantData_frame = m_dataBeingRenderedByRenderThread->constantData_frame;
				m_constantBuffer_frame.Update(&constantData_frame);
			}

			BindAndDrawInRenderFrame();

			// Everything has been drawn to the "back buffer", which is just an image in memory.
			// In order to display it the contents of the back buffer must be "presented"
			// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
			{
				const auto deviceContext = sContext::g_context.deviceContext;
				EAE6320_ASSERT(deviceContext != NULL);
				const auto glResult = SwapBuffers(deviceContext);
				EAE6320_ASSERT(glResult != FALSE);
			}

			// Once everything has been drawn the data that was submitted for this frame
			// should be cleaned up and cleared.
			CleanSubmittedData();
			// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
			{
				// (At this point in the class there isn't anything that needs to be cleaned up)
			}
		}
		eae6320::cResult View::Initialize(const sInitializationParameters& i_initializationParameters)
		{
			auto result = Results::Success;

			// Initialize the platform-specific context
			if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
			{
				EAE6320_ASSERTF(false, "Can't initialize Graphics without context");
				return result;
			}
			// Initialize the asset managers
			{
				if (!(result = cRenderState::s_manager.Initialize()))
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without the render state manager");
					return result;
				}
				if (!(result = cShader::s_manager.Initialize()))
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without the shader manager");
					return result;
				}
			}
			// Initialize the platform-independent graphics objects
			{
				if (result = m_constantBuffer_frame.Initialize())
				{
					// There is only a single frame constant buffer that is reused
					// and so it can be bound at initialization time and never unbound
					m_constantBuffer_frame.Bind(
						// In our class both vertex and fragment shaders use per-frame constant data
						ShaderTypes::Vertex | ShaderTypes::Fragment);
				}
				else
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without frame constant buffer");
					return result;
				}
			}
			// Initialize the events
			{
				if (!(result = m_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data has been submitted from the application thread");
					return result;
				}
				if (!(result = m_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
					Concurrency::EventState::Signaled)))
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data can be submitted from the application thread");
					return result;
				}
			}
			// Initialize the shading data
			{
				if (!(result = InitializeShadingData()))
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
					return result;
				}
			}
			// Initialize the geometry
			{
				if (!(result = InitializeGeometry()))
				{
					EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
					return result;
				}
			}

			return result;
		}
		eae6320::cResult View::CleanUp()
		{
			auto result = Results::Success;
			{
				const auto result_constantBuffer_frame = m_constantBuffer_frame.CleanUp();
				if (!result_constantBuffer_frame)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_constantBuffer_frame;
					}
				}
			}

			{
				const auto result_shaderManager = cShader::s_manager.CleanUp();
				if (!result_shaderManager)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_shaderManager;
					}
				}
			}
			{
				const auto result_renderStateManager = cRenderState::s_manager.CleanUp();
				if (!result_renderStateManager)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_renderStateManager;
					}
				}
			}

			{
				const auto result_context = sContext::g_context.CleanUp();
				if (!result_context)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_context;
					}
				}
			}
			return result;
		}
	}
}