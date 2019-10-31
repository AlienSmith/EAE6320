#include "cEffectBuilder.h"
#include "Engine/Platform/Platform.h"
#include "Tools/AssetBuildLibrary/Functions.h"
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

eae6320::cResult eae6320::Assets::cEffectBuilder::LoadAsset(const char* const i_path, lua_State*& io_luaState)
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

eae6320::cResult eae6320::Assets::cEffectBuilder::LoadShader(lua_State& io_luaState, const char*& vertex_shader, const char*& fragment_shader)
{
	auto result = eae6320::Results::Success;
	constexpr auto* const vertex_Shader_Path = "VertexShaderPath";
	constexpr auto* const fragment_Shader_Path = "FragmentShaderPath";
	lua_pushstring(&io_luaState, vertex_Shader_Path);
	lua_gettable(&io_luaState, -2);
	if (lua_isstring(&io_luaState, -1)) {
		vertex_shader = lua_tostring(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
	}lua_pushstring(&io_luaState, fragment_Shader_Path);
	lua_gettable(&io_luaState, -2);
	if (lua_isstring(&io_luaState, -1)) {
		fragment_shader = lua_tostring(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
	}
	return result;
}

eae6320::cResult eae6320::Assets::cEffectBuilder::Load(const std::string& i_path, const std::string& o_path)
{
	auto result = Results::Success;
	lua_State* temp_state = nullptr;
	const char* vertexshader = nullptr;
	const char* fragmentshader = nullptr;
	//I will not need them After I write them to the runtime lua files
	eae6320::cScopeGuard scopeGuard_onExit([&temp_state, &vertexshader, &fragmentshader]
		{
			if (temp_state)
			{
				lua_close(temp_state);
				temp_state = nullptr;
				//you are not responsible for those memories
				//if (vertexshader != nullptr) {
				//	delete[] vertexshader;
				//	vertexshader = nullptr;
				//}if (fragmentshader != nullptr) {
				//	delete[] fragmentshader;
				//	fragmentshader = nullptr;
				//}
			}
		});
	result = LoadAsset(i_path.c_str(), temp_state);
	if (!result.IsSuccess()) {
		eae6320::Assets::OutputErrorMessageWithFileInfo("Failed to load shader from file %s", i_path.c_str());
		return result;
	}
	LoadShader(*temp_state, vertexshader, fragmentshader);
	//Write out the bineray file here
	const char null = '\0';
	std::string prefix = std::string("data/");
	std::ofstream outfile(o_path, std::ofstream::binary);
	//outfile.write(vertexshader, strlen(vertexshader));
	//outfile.write(&null, 1);
	//outfile.write(fragmentshader, strlen(fragmentshader));
	//outfile.write(&null, 1);
	std::string vertex_path;
	std::string fragment_path;
	ConvertSourceRelativePathToBuiltRelativePath(vertexshader,"effects", vertex_path);
	ConvertSourceRelativePathToBuiltRelativePath(fragmentshader, "effects", fragment_path);
	outfile.write(prefix.c_str(), prefix.length());
	outfile.write(vertex_path.c_str(), vertex_path.length());
	outfile.write(&null, 1);
	outfile.write(prefix.c_str(), prefix.length());
	outfile.write(fragment_path.c_str(), fragment_path.length());
	outfile.write(&null, 1);
	return result;
}

eae6320::cResult eae6320::Assets::cEffectBuilder::Build(const std::vector<std::string>& i_arguments)
{	
	//auto result = eae6320::Results::Success;
	eae6320::cResult result = Load(m_path_source,m_path_target);
	//eae6320::cResult result = Platform::CopyFileW(m_path_source, m_path_target, false, true);
	return result;
}
