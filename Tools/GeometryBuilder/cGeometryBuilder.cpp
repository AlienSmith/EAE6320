#include "cGeometryBuilder.h"
#include "Engine/Platform/Platform.h"
#include "Tools/AssetBuildLibrary/Functions.h"
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
eae6320::cResult eae6320::Assets::cGeometryBuilder::LoadAsset(const char* const i_path, lua_State*& io_luaState)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			result = eae6320::Results::OutOfMemory;
			eae6320::Assets::OutputErrorMessage("Failed to create a new Lua state");
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
			eae6320::Assets::OutputErrorMessage(lua_tostring(luaState, -1));
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
					eae6320::Assets::OutputErrorMessage("Asset files must return a table )");
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					return result;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				eae6320::Assets::OutputErrorMessage("Asset files must return a single table )");
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				return result;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			eae6320::Assets::OutputErrorMessage(lua_tostring(luaState, -1));
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}
	io_luaState = luaState;
	return result;
}
eae6320::cResult eae6320::Assets::cGeometryBuilder::LoadTableValues_Vertexs(lua_State& io_luaState, eae6320::Graphics::VertexFormats::s3dObject*& vertex_Data, uint16_t& vertex_count)
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
		const auto VertexCount = luaL_len(&io_luaState, -1);
		vertex_count = (int)VertexCount;
		eae6320::Graphics::VertexFormats::s3dObject* object = new eae6320::Graphics::VertexFormats::s3dObject[vertex_count];
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
		std::stringstream buffer;
		buffer << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")";
		eae6320::Assets::OutputErrorMessage(buffer.str().c_str());
		return result;
	}
	return result;
}
eae6320::cResult eae6320::Assets::cGeometryBuilder::LoadTableValues_Index(lua_State& io_luaState, uint16_t*& index_Data, uint16_t& index_Count)
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
		const auto IndexCount = luaL_len(&io_luaState, -1);
		index_Count = (int)IndexCount;
		uint16_t* object = new uint16_t[index_Count];
		for (int i = 1; i <= IndexCount; ++i) {
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			object[i - 1] = (int)lua_tonumber(&io_luaState, -1);
			cScopeGuard pop([&io_luaState] {lua_pop(&io_luaState, 1); });
		}
		index_Data = object;
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::stringstream buffer;
		buffer << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")";
		eae6320::Assets::OutputErrorMessage(buffer.str().c_str());
		return result;
	}
	return result;
}
eae6320::cResult eae6320::Assets::cGeometryBuilder::Load(const std::string& i_path, const std::string& o_path)
{
	auto result = Results::Success;
	lua_State* temp_state = nullptr;
	uint16_t vertexcount = 0;
	eae6320::Graphics::VertexFormats::s3dObject* vertexData = nullptr;
	uint16_t indexcount = 0;
	uint16_t* indexdata = nullptr;
	eae6320::cScopeGuard scopeGuard_onExit([&temp_state, &vertexData, &indexdata]
		{
			if (temp_state)
			{
				lua_close(temp_state);
				temp_state = nullptr;
			}
			if (indexdata) {
				delete[] indexdata;
			}
			if (vertexData) {
				delete[] vertexData;
			}
		});
	result = LoadAsset(i_path.c_str(), temp_state);
	if (!result.IsSuccess()) {
		eae6320::Assets::OutputErrorMessageWithFileInfo("Failed to load shader from file %s", i_path.c_str());
		return result;
	}
	LoadTableValues_Vertexs(*temp_state, vertexData, vertexcount);
	LoadTableValues_Index(*temp_state, indexdata, indexcount);
	//Write out the bineray file here
	std::ofstream outfile(o_path, std::ofstream::binary);
	outfile.write((const char*)& vertexcount, sizeof(uint16_t));
	outfile.write((const char*)& indexcount,sizeof(uint16_t));
	size_t vertex_data_size = vertexcount * sizeof(eae6320::Graphics::VertexFormats::s3dObject);
	outfile.write((const char*)vertexData, vertex_data_size);
	size_t index_data_size = indexcount * sizeof(uint16_t);
	outfile.write((const char*)indexdata, index_data_size);
	return result;
}
eae6320::cResult eae6320::Assets::cGeometryBuilder::Build(const std::vector<std::string>& i_arguments)
{

	//eae6320::cResult result = Platform::CopyFileW(m_path_source, m_path_target, false, true);
	eae6320::cResult result = Load(m_path_source, m_path_target);
	if (!result.IsSuccess()) {
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Build Geoemtry failed");
	}
	return result;
}