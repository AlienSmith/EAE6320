#pragma once
#include <Tools/AssetBuildLibrary/cbBuilder.h>
#include <External/Lua/Includes.h>
#include <Engine/Graphics/VertexFormats.h>
namespace eae6320
{
	namespace Assets
	{
		class cEffectBuilder : public cbBuilder
		{
			// Inherited Implementation
			//=========================
		public:
			static eae6320::cResult LoadAsset(const char* const i_path, lua_State*& io_luaState);
			static eae6320::cResult LoadShader(lua_State& io_luaState, const char*& vertex_shader,const char*& fragment_shader);
			static eae6320::cResult Load(const std::string& i_path, const std::string& o_path);
		private:

			// Build
			//------

			virtual cResult Build(const std::vector<std::string>& i_arguments) override;

			// Implementation
			//===============
		};
	}
}

