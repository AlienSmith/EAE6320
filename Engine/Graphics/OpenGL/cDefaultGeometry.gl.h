#pragma once
#include "../Graphics.h"
#include "Includes.h"
namespace eae6320 {
		class DefaultGeometry {
		public:
			DefaultGeometry():m_vertexBufferId(0),
			m_vertexArrayId(0){}
			~DefaultGeometry() { CleanUp(); }
			GLuint m_vertexBufferId = 0;
			GLuint m_vertexArrayId = 0;
			eae6320::cResult CleanUp();
			eae6320::cResult InitializeGeometry();
		};
}