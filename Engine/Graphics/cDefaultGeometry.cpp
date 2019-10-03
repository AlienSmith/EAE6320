#include "cDefaultGeometry.h"
namespace eae6320 {
	namespace Graphics {
		eae6320::Assets::cManager<eae6320::Graphics::DefaultGeometry> eae6320::Graphics::DefaultGeometry::s_manager;
		cResult eae6320::Graphics::DefaultGeometry::Load(const std::string& i_path, DefaultGeometry*& o_instance)
		{
			auto result = Results::Success;

			Platform::sDataFromFile dataFromFile;
			// Load the binary data
			{
				std::string errorMessage;
				if (!(result = Platform::LoadBinaryFile(i_path.c_str(), dataFromFile, &errorMessage)))
				{
					EAE6320_ASSERTF(false, errorMessage.c_str());
					Logging::OutputError("Failed to load shader from file %s: %s", i_path.c_str(), errorMessage.c_str());
					return result;
				}
			}
			//Load the file here
			// Call the create function here
			//test data
			sDataRequriedToIntializeObject triangle;
			triangle.indexcount = 3;
			triangle.vertexcount = 3;
			eae6320::Graphics::VertexFormats::s3dObject triangle_vertexData[3];
			{

				triangle_vertexData[0].x = 0.0f;
				triangle_vertexData[0].y = 0.0f;
				triangle_vertexData[0].z = 0.0f;

				triangle_vertexData[1].x = 1.0f;
				triangle_vertexData[1].y = 0.0f;
				triangle_vertexData[1].z = 0.0f;

				triangle_vertexData[2].x = 1.0f;
				triangle_vertexData[2].y = 1.0f;
				triangle_vertexData[2].z = 0.0f;

			}
			triangle.vertexData = triangle_vertexData;
			uint16_t triangle_indexData[3]{ 0,1,2 };
			triangle.indexdata = triangle_indexData;
			DefaultGeometry::Create(triangle, o_instance);

			return result;
		}
	}
}
