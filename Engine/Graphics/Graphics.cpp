#include "View.h"
namespace eae6320 {
	namespace Graphics {
		static View view;
		void SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime) {
			return view.SubmitElapsedTime(i_elapsedSecondCount_systemTime, i_elapsedSecondCount_simulationTime);
		}
		void SubmitClearColor(float r, float g, float b, float alpha)
		{
			view.SubmitBackGroundColor(r, g, b, alpha);
		}
		void SubmitEffectWithObject(cEffect* effect, DefaultGeometry* geometry)
		{
			view.SubmitEffectWithObject(effect, geometry);
		}
		void SubmitCameraData(const sCameradata& cameradata)
		{
			view.SubmitCameraPerspectiveData(cameradata);
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
			cResult result = eae6320::Results::Success;
			//float color[4] = { 1.0f,0.0f,0.0f,0.0f };
			//view.SetClearColor(color);
			view.Initialize(i_initializationParameters);
			//sDataRequriedToIntializeObject squre;
			//squre.indexcount = 6;
			//squre.vertexcount = 4;
			//eae6320::Graphics::VertexFormats::s3dObject temp_vertexData[4];
			//{
			//	temp_vertexData[0].x = 0.0f;
			//	temp_vertexData[0].y = 0.0f;
			//	temp_vertexData[0].z = 0.0f;

			//	temp_vertexData[1].x = 1.0f;
			//	temp_vertexData[1].y = 0.0f;
			//	temp_vertexData[1].z = 0.0f;

			//	temp_vertexData[2].x = 1.0f;
			//	temp_vertexData[2].y = 1.0f;
			//	temp_vertexData[2].z = 0.0f;

			//	temp_vertexData[3].x = 0.0f;
			//	temp_vertexData[3].y = 1.0f;
			//	temp_vertexData[3].z = 0.0f;
			//}
			//squre.vertexData = temp_vertexData;
			//uint16_t temp_indexData[6]{ 0,1,2,0,2,3 };
			//squre.indexdata = temp_indexData;
			//DefaultGeometry::Create(squre, view.m_Geometry_Array[0]);

			//sDataRequriedToIntializeObject triangle;
			//triangle.indexcount = 3;
			//triangle.vertexcount = 3;
			//eae6320::Graphics::VertexFormats::s3dObject triangle_vertexData[3];
			//{
			//	triangle_vertexData[0].x = 0.0f;
			//	triangle_vertexData[0].y = 0.0f;
			//	triangle_vertexData[0].z = 0.0f;

			//	triangle_vertexData[1].x = -1.0f;
			//	triangle_vertexData[1].y = -1.0f;
			//	triangle_vertexData[1].z = 0.0f;

			//	triangle_vertexData[2].x = 0.0f;
			//	triangle_vertexData[2].y = -1.0f;
			//	triangle_vertexData[2].z = 0.0f;
			//}
			//triangle.vertexData = triangle_vertexData;
			//uint16_t triangle_indexData[3]{ 0,1,2 };
			//triangle.indexdata = triangle_indexData;
			//DefaultGeometry::Create(triangle, view.m_Geometry_Array[1]);

			//sDataRequriedToIntializeEffect flesh_data;
			//flesh_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
			//flesh_data.fragment_shader_path = "data/Shaders/Fragment/test.shader";
			//cEffect::Create(flesh_data, view.m_Effect_Array[0]);

			//sDataRequriedToIntializeEffect white_data;
			//white_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
			//white_data.fragment_shader_path = "data/Shaders/Fragment/standard.shader";
			//cEffect::Create(white_data, view.m_Effect_Array[1]);
			return result;
		}
		cResult CleanUp() {
			return view.CleanUp();
		}
	}
}