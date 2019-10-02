#pragma once
/*
	This class builds geometries
*/

#ifndef EAE6320_CGEOMETRYBUILDER_H
#define EAE6320_CGEOMETRYBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/cbBuilder.h>


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
