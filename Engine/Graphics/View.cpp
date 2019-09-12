#include "View.h"
#include "cDefaultGeometry.h"
#include "cEffect.h"
namespace eae6320 {
	namespace Graphics {
		//Define the data here
		View::~View() { 
			//CleanUp(); 
		}
		eae6320::cResult View::InitializeGeometry() {
			sDataRequriedToIntializeObject squre;
			squre.indexcount = 6;
			squre.vertexcount = 4;
			eae6320::Graphics::VertexFormats::s3dObject temp_vertexData[4];
			{
				temp_vertexData[0].x = 0.0f;
				temp_vertexData[0].y = 0.0f;
				temp_vertexData[0].z = 0.0f;

				temp_vertexData[1].x = 1.0f;
				temp_vertexData[1].y = 0.0f;
				temp_vertexData[1].z = 0.0f;

				temp_vertexData[2].x = 1.0f;
				temp_vertexData[2].y = 1.0f;
				temp_vertexData[2].z = 0.0f;

				temp_vertexData[3].x = 0.0f;
				temp_vertexData[3].y = 1.0f;
				temp_vertexData[3].z = 0.0f;
			}
			squre.vertexData = temp_vertexData;
			uint16_t temp_indexData[6]{ 0,1,2,0,2,3 };
			squre.indexdata = temp_indexData;
			return m_defaultGeometry.InitializeGeometry(squre);
		}
		eae6320::cResult View::InitializeShadingData() {
			sDataRequriedToIntializeEffect flesh_data;
			flesh_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
			flesh_data.fragment_shader_path = "data/Shaders/Fragment/test.shader";
			return m_cEffect.InitializeShadingData(flesh_data);
		}
		void View::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime) {
			EAE6320_ASSERT(m_dataBeingSubmittedByApplicationThread);
			auto& constantData_frame = m_dataBeingSubmittedByApplicationThread->constantData_frame;
			constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
			constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
		}
		eae6320::cResult View::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
		{
			return Concurrency::WaitForEvent(m_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
		}
		eae6320::cResult View::SignalThatAllDataForAFrameHasBeenSubmitted()
		{
			return m_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
		}

		bool View::RenderFrameCheck()
		{
			const auto result = Concurrency::WaitForEvent(m_whenAllDataHasBeenSubmittedFromApplicationThread);
			if (result)
			{
				// Switch the render data pointers so that
				// the data that the application just submitted becomes the data that will now be rendered
				std::swap(m_dataBeingSubmittedByApplicationThread, m_dataBeingRenderedByRenderThread);
				// Once the pointers have been swapped the application loop can submit new data
				const auto result = m_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal();
				if (!result)
				{
					EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
					Logging::OutputError("Failed to signal that new render data can be submitted");
					UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
						" The application is probably in a bad state and should be exited");
					return false;
				}
			}
			else
			{
				EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
				Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
				UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
					" The application is probably in a bad state and should be exited");
				return false;
			}
			return true;
		}
	}
}