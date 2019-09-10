#include "View.h"
namespace eae6320 {
	namespace Graphics {
		static View view;
		void SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime) {
			return view.SubmitElapsedTime(i_elapsedSecondCount_systemTime, i_elapsedSecondCount_simulationTime);
		}
		cResult WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds) {
			return view.WaitUntilDataForANewFrameCanBeSubmitted(i_timeToWait_inMilliseconds);
		}
		cResult SignalThatAllDataForAFrameHasBeenSubmitted() {
			return view.SignalThatAllDataForAFrameHasBeenSubmitted();
		}
		void RenderFrame() {
			return view.RenderFrame();
		}
		cResult Initialize(const sInitializationParameters& i_initializationParameters) {
			return view.Initialize(i_initializationParameters);
		}
		cResult CleanUp() {
			return view.CleanUp();
		}
	}
}