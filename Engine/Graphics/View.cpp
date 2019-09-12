#include "View.h"
namespace eae6320 {
	namespace Graphics {
		View::~View() { 
			//CleanUp(); 
		}
		eae6320::cResult View::InitializeGeometry() {
			return m_defaultGeometry.InitializeGeometry();
		}
		eae6320::cResult View::InitializeShadingData() {
			return m_cEffect.InitializeShadingData();
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