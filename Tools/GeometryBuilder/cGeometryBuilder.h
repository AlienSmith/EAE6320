#pragma once
/*
	This class builds geometries
*/

#ifndef EAE6320_CGEOMETRYBUILDER_H
#define EAE6320_CGEOMETRYBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/cbBuilder.h>
#include <External/Lua/Includes.h>
#include <Engine/Graphics/VertexFormats.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cGeometryBuilder : public cbBuilder
		{
			// Inherited Implementation
			//=========================
		public:
			static eae6320::cResult LoadAsset(const char* const i_path, lua_State*& io_luaState);
			static eae6320::cResult LoadTableValues_Vertexs(lua_State& io_luaState, eae6320::Graphics::VertexFormats::s3dObject*& vertex_Data, uint16_t& vertex_count);
			static eae6320::cResult LoadTableValues_Index(lua_State& io_luaState, uint16_t*& index_Data, uint16_t& index_Count);
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

#endif	// EAE6320_CSHADERBUILDER_H
