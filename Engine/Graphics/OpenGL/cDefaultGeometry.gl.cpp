#include "../cDefaultGeometry.h"
#include "../cConstantBuffer.h"
#include "../ConstantBufferFormats.h"
#include "../cRenderState.h"
#include "../cShader.h"
#include "../sContext.h"
#include "../VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>
#include <cstdint>
namespace eae6320 {
	namespace Graphics {
		DefaultGeometry::DefaultGeometry() :m_vertexBufferId(0),
			m_vertexArrayId(0) {}
		DefaultGeometry::~DefaultGeometry() {
			CleanUp();
		}
		eae6320::cResult DefaultGeometry::CleanUp() {
			auto result = Results::Success;
			if (m_vertexArrayId != 0)
			{
				// Make sure that the vertex array isn't bound
				{
					// Unbind the vertex array
					glBindVertexArray(0);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						if (result)
						{
							result = Results::Failure;
						}
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
				}
				constexpr GLsizei arrayCount = 1;
				glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex array: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				m_vertexArrayId = 0;
			}
			if (m_vertexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &m_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				m_vertexBufferId = 0;
			}
			//Clean up the added indexbufferid and indexCountToRender
			if (m_indexBufferId != 0)
			{
				constexpr GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &m_indexBufferId);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to delete the index buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
				m_indexBufferId = 0;
			}
			if (indexCountToRender != 0) {
				indexCountToRender = 0;
			}
			return result;
		}
		eae6320::cResult DefaultGeometry::InitializeGeometry(const sDataRequriedToIntializeObject& data) {
			auto result = eae6320::Results::Success;

			// Create a vertex array object and make it active
			{
				constexpr GLsizei arrayCount = 1;
				glGenVertexArrays(arrayCount, &m_vertexArrayId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindVertexArray(m_vertexArrayId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Create a vertex buffer object and make it active
			{
				constexpr GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &m_vertexBufferId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Assign the data to the buffer
			{
				//constexpr unsigned int triangleCount = 1;
				//constexpr unsigned int vertexCountPerTriangle = 4;
				//const auto vertexCount = 4;
				//eae6320::Graphics::VertexFormats::s3dObject vertexData[vertexCount];
				//{
				//	vertexData[0].x = 0.0f;
				//	vertexData[0].y = 0.0f;
				//	vertexData[0].z = 0.0f;

				//	vertexData[1].x = 1.0f;
				//	vertexData[1].y = 0.0f;
				//	vertexData[1].z = 0.0f;

				//	vertexData[2].x = 1.0f;
				//	vertexData[2].y = 1.0f;
				//	vertexData[2].z = 0.0f;

				//	vertexData[3].x = 0.0f;
				//	vertexData[3].y = 1.0f;
				//	vertexData[3].z = 0.0f;
				//}
				const auto bufferSize = data.vertexcount * sizeof(*data.vertexData);
				EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(GLsizeiptr) * 8)));
				glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(data.vertexData),
					// In our class we won't ever read from the buffer
					GL_STATIC_DRAW);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Create a index buffer object and make it active
			{
				constexpr GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &m_indexBufferId);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Assign the data to the buffer
			{
				// Data need to draw
				indexCountToRender = data.indexcount;
				/*constexpr unsigned int triangleCount = 2;
				constexpr unsigned int vertexCountPerTriangle = 3;
				const auto indexCount = triangleCount * vertexCountPerTriangle;
				uint16_t indexData[indexCount]{ 0,1,2,0,2,3 };*/
				const auto bufferSize = data.indexcount * sizeof(*data.indexdata);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(data.indexdata),
					// In our class we won't ever read from the buffer
					GL_STATIC_DRAW);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Initialize vertex format
			{
				// The "stride" defines how large a single vertex is in the stream of data
				// (or, said another way, how far apart each position element is)
				const auto stride = static_cast<GLsizei>(sizeof(eae6320::Graphics::VertexFormats::s3dObject));

				// Position (0)
				// 3 floats == 12 bytes
				// Offset = 0
				{
					constexpr GLuint vertexElementLocation = 0;
					constexpr GLint elementCount = 3;
					constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
						reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::s3dObject, x)));
					const auto errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode != GL_NO_ERROR)
						{
							result = eae6320::Results::Failure;
							EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
								vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							return result;
						}
					}
					else
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						eae6320::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
							vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return result;
					}
				}
			}

			return result;
		}
		void DefaultGeometry::Draw() {
			// Bind a specific vertex buffer to the device as a data source
			{
				EAE6320_ASSERT(m_vertexArrayId != 0);
				glBindVertexArray(m_vertexArrayId);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
			//Render triangles from the currently-bound index buffer
			{
				// The mode defines how to interpret multiple vertices as a single "primitive";
				// a triangle list is defined
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				constexpr GLenum mode = GL_TRIANGLES;
				// It's possible to start rendering primitives in the middle of the stream
				const GLvoid* const offset = 0;
				glDrawElements(mode, static_cast<GLsizei>(indexCountToRender), GL_UNSIGNED_SHORT, offset);
				EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			}
			//// Render triangles from the currently-bound vertex buffer
			//{
			//	// The mode defines how to interpret multiple vertices as a single "primitive";
			//	// a triangle list is defined
			//	// (meaning that every primitive is a triangle and will be defined by three vertices)
			//	constexpr GLenum mode = GL_TRIANGLES;
			//	// As of this comment only a single triangle is drawn
			//	// (you will have to update this code in future assignments!)
			//	constexpr unsigned int triangleCount = 2;
			//	constexpr unsigned int vertexCountPerTriangle = 3;
			//	constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
			//	// It's possible to start rendering primitives in the middle of the stream
			//	constexpr unsigned int indexOfFirstVertexToRender = 0;
			//	glDrawArrays(mode, indexOfFirstVertexToRender, vertexCountToRender);
			//	EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			//}
		}
	}
}
