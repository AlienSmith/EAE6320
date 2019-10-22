#include "cDefaultGeometry.h"
#include <iostream>
namespace eae6320 {
	namespace Graphics {
		eae6320::Assets::cManager<eae6320::Graphics::DefaultGeometry> eae6320::Graphics::DefaultGeometry::s_manager;
		eae6320::cResult DefaultGeometry::LoadAsset(const char* const i_path, lua_State*& io_luaState)
		{
			auto result = eae6320::Results::Success;

			// Create a new Lua state
			lua_State* luaState = nullptr;
			{
				luaState = luaL_newstate();
				if (!luaState)
				{
					result = eae6320::Results::OutOfMemory;
					std::cerr << "Failed to create a new Lua state" << std::endl;
					return result;
				}
			}

			// Load the asset file as a "chunk",
			// meaning there will be a callable function at the top of the stack
			const auto stackTopBeforeLoad = lua_gettop(luaState);
			{
				const auto luaResult = luaL_loadfile(luaState, i_path);
				if (luaResult != LUA_OK)
				{
					result = eae6320::Results::Failure;
					std::cerr << lua_tostring(luaState, -1) << std::endl;
					// Pop the error message
					lua_pop(luaState, 1);
					return result;
				}
			}
			// Execute the "chunk", which should load the asset
			// into a table at the top of the stack
			{
				constexpr int argumentCount = 0;
				constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
				constexpr int noMessageHandler = 0;
				const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
				if (luaResult == LUA_OK)
				{
					// A well-behaved asset file will only return a single value
					const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
					if (returnedValueCount == 1)
					{
						// A correct asset file _must_ return a table
						if (!lua_istable(luaState, -1))
						{
							result = eae6320::Results::InvalidFile;
							std::cerr << "Asset files must return a table (instead of a " <<
								luaL_typename(luaState, -1) << ")" << std::endl;
							// Pop the returned non-table value
							lua_pop(luaState, 1);
							return result;
						}
					}
					else
					{
						result = eae6320::Results::InvalidFile;
						std::cerr << "Asset files must return a single table (instead of " <<
							returnedValueCount << " values)" << std::endl;
						// Pop every value that was returned
						lua_pop(luaState, returnedValueCount);
						return result;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << lua_tostring(luaState, -1) << std::endl;
					// Pop the error message
					lua_pop(luaState, 1);
					return result;
				}
			}
			io_luaState = luaState;
			return result;
		}
		eae6320::cResult DefaultGeometry::LoadTableValues_Vertexs(lua_State& io_luaState, eae6320::Graphics::VertexFormats::s3dObject*& vertex_Data, int& vertex_count)
		{
			auto result = eae6320::Results::Success;
			constexpr auto* const key = "VertexData";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);
			eae6320::cScopeGuard scopeGuard_popTextures([&io_luaState]
				{
					lua_pop(&io_luaState, 1);
				});
			if (lua_istable(&io_luaState, -1))
			{
				std::cout << "Iterating through every texture path:" << std::endl;
				const auto VertexCount = luaL_len(&io_luaState, -1);
				vertex_count = (int)VertexCount;
				eae6320::Graphics::VertexFormats::s3dObject* object = new eae6320::Graphics::VertexFormats::s3dObject[vertex_count];
				EAE6320_ASSERTF(vertex_count <500000, "Too Many Vertex");
				Logging::OutputError("Too Many Vertex");
				for (int i = 1; i <= vertex_count; ++i) {
					lua_pushinteger(&io_luaState, i);
					lua_gettable(&io_luaState, -2);
					eae6320::cScopeGuard scopeGuard_popTexturePath([&io_luaState]
						{
							lua_pop(&io_luaState, 1);
						});
					lua_pushinteger(&io_luaState, 1);
					lua_gettable(&io_luaState, -2);
					object[i - 1].x = (float)lua_tonumber(&io_luaState, -1);
					lua_pop(&io_luaState, 1);
					lua_pushinteger(&io_luaState, 2);
					lua_gettable(&io_luaState, -2);
					object[i - 1].y = (float)lua_tonumber(&io_luaState, -1);
					lua_pop(&io_luaState, 1);
					lua_pushinteger(&io_luaState, 3);
					lua_gettable(&io_luaState, -2);
					object[i - 1].z = (float)lua_tonumber(&io_luaState, -1);
					lua_pop(&io_luaState, 1);
				}
				vertex_Data = object;
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "The value at \"" << key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
				return result;
			}
			return result;
		}
		eae6320::cResult DefaultGeometry::LoadTableValues_Index(lua_State& io_luaState, uint16_t*& index_Data, int& index_Count)
		{
			auto result = eae6320::Results::Success;
			constexpr auto* const key = "IndexData";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);
			eae6320::cScopeGuard scopeGuard_popTextures([&io_luaState]
				{
					lua_pop(&io_luaState, 1);
				});
			if (lua_istable(&io_luaState, -1))
			{
				std::cout << "Iterating through every texture path:" << std::endl;
				const auto IndexCount = luaL_len(&io_luaState, -1);
				index_Count = (int)IndexCount;
				uint16_t* object = new uint16_t[index_Count];
				for (int i = 1; i <= IndexCount; ++i) {
					lua_pushinteger(&io_luaState, i);
					lua_gettable(&io_luaState, -2);
					object[i-1] = (int)lua_tonumber(&io_luaState, -1);
					cScopeGuard pop([&io_luaState] {lua_pop(&io_luaState, 1); });
				}
				index_Data = object;
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "The value at \"" << key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
				return result;
			}
			return result;
		}
		cResult eae6320::Graphics::DefaultGeometry::Load(const std::string& i_path, DefaultGeometry*& o_instance)
		{
			auto result = Results::Success;

			//Platform::sDataFromFile dataFromFile;
			//// Load the binary data
			//{
			//	std::string errorMessage;
			//	if (!(result = Platform::LoadBinaryFile(i_path.c_str(), dataFromFile, &errorMessage)))
			//	{
			//		EAE6320_ASSERTF(false, errorMessage.c_str());
			//		Logging::OutputError("Failed to load shader from file %s: %s", i_path.c_str(), errorMessage.c_str());
			//		return result;
			//	}
			//}
			//Load the file here
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

			std::string errorMessage;
			lua_State* temp_state = nullptr;
			sDataRequriedToIntializeObject geometry;
			eae6320::cScopeGuard scopeGuard_onExit([&temp_state,&geometry]
				{
					if (temp_state)
					{
						lua_close(temp_state);
						temp_state = nullptr;
					}
					if (geometry.indexdata) {
						delete[] geometry.indexdata;
					}
					if (geometry.vertexData) {
						delete[] geometry.vertexData;
					}
				});
			result = LoadAsset(i_path.c_str(), temp_state);
			if (!result.IsSuccess()) {
				EAE6320_ASSERTF(false, errorMessage.c_str());
				Logging::OutputError("Failed to load shader from file %s: %s", i_path.c_str(), errorMessage.c_str());
				return result;
			}
			LoadTableValues_Vertexs(*temp_state, geometry.vertexData, geometry.vertexcount);
			LoadTableValues_Index(*temp_state, geometry.indexdata, geometry.indexcount);
			// Call the create function here
			//test data
			DefaultGeometry::Create(geometry, o_instance);
			return result;
		}
	}
}
