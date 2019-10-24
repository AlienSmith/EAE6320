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
		DefaultGeometry::DefaultGeometry():m_vertexFormat(eae6320::Graphics::cVertexFormat::Handle()),
			m_vertexBuffer(nullptr),
			m_indexBuffer(nullptr){}
		DefaultGeometry::~DefaultGeometry()
		{
			CleanUp();
		}
		eae6320::cResult DefaultGeometry::CleanUp() {
			auto result = Results::Success;
			if (m_vertexBuffer)
			{
				m_vertexBuffer->Release();
				m_vertexBuffer = nullptr;
			}
			if (m_vertexFormat)
			{
				const auto result_vertexFormat = eae6320::Graphics::cVertexFormat::s_manager.Release(m_vertexFormat);
				if (!result_vertexFormat)
				{
					EAE6320_ASSERT(false);
					if (result)
					{
						result = result_vertexFormat;
					}
				}
			}
			//Clean up added variables
			if (m_indexBuffer) {
				m_indexBuffer->Release();
				m_indexBuffer = nullptr;
			}
			if (indexCountToRender != 0) {
				indexCountToRender = 0;
			}
			return result;
		}
		eae6320::cResult DefaultGeometry::InitializeGeometry(const sDataRequriedToIntializeObject& data) {
			auto result = eae6320::Results::Success;

			auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
			EAE6320_ASSERT(direct3dDevice);

			// Vertex Format
			{
				if (!(result = eae6320::Graphics::cVertexFormat::s_manager.Load(eae6320::Graphics::VertexTypes::_3dObject, m_vertexFormat,
					"data/Shaders/Vertex/vertexInputLayout_3dObject.shader")))
				{
					EAE6320_ASSERTF(false, "Can't initialize geometry without vertex format");
					return result;
				}
			}
			// Vertex Buffer
			//Change triangleCount to 2
			{
				//constexpr unsigned int triangleCount = 2;
				//constexpr unsigned int vertexCountPerTriangle = 3;
				//constexpr auto vertexCount = triangleCount * vertexCountPerTriangle;
				//eae6320::Graphics::VertexFormats::s3dObject vertexData[vertexCount];
				//{ // 123 134
				//	vertexData[0].x = 0.0f;
				//	vertexData[0].y = 0.0f;
				//	vertexData[0].z = 0.0f;

				//	vertexData[1].x = 0.0f;
				//	vertexData[1].y = 1.0f;
				//	vertexData[1].z = 0.0f;

				//	vertexData[2].x = 1.0f;
				//	vertexData[2].y = 1.0f;
				//	vertexData[2].z = 0.0f;

				//	vertexData[3].x = 1.0f;
				//	vertexData[3].y = 0.0f;
				//	vertexData[3].z = 0.0f;

				//}
				D3D11_BUFFER_DESC bufferDescription{};
				{
					const auto bufferSize = data.vertexcount * sizeof(eae6320::Graphics::VertexFormats::s3dObject);
					EAE6320_ASSERT(bufferSize < (uint64_t(1u) << (sizeof(bufferDescription.ByteWidth) * 8)));
					bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
					bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
					bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
					bufferDescription.MiscFlags = 0;
					bufferDescription.StructureByteStride = 0;	// Not used
				}
				D3D11_SUBRESOURCE_DATA initialData{};
				{
					initialData.pSysMem = data.vertexData;
					// (The other data members are ignored for non-texture buffers)
				}

				const auto d3dResult = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "3D object vertex buffer creation failed (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create a 3D object vertex buffer (HRESULT %#010x)", d3dResult);
					return result;
				}
			}
			//index Buffer
			{
				indexCountToRender = data.indexcount;
				//indexCountToRender = data.indexcount > 3? 6:data.indexcount;
				//constexpr unsigned int indexCount = 6;
				//0,1,2,0,2,3 to 0,2,1,0,3,2 right handed to left handed
				/*for (int i = 0; i < indexCountToRender / 3.0f; i++) {
					uint16_t temp_int = data.indexdata[i * 3 + 1];
					data.indexdata[i * 3 + 1] = data.indexdata[i * 3 + 2];
					data.indexdata[i * 3 + 2] = temp_int;
				}*/				
				D3D11_BUFFER_DESC bufferDescription{};
				{
					const auto bufferSize = indexCountToRender * sizeof(uint16_t);
					bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
					bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
					bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
					bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
					bufferDescription.MiscFlags = 0;
					bufferDescription.StructureByteStride = 0;	// Not used
				}
				D3D11_SUBRESOURCE_DATA initialData{};
				{
					initialData.pSysMem = data.indexdata;
					// (The other data members are ignored for non-texture buffers)
				}
				const auto d3dResult = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_indexBuffer);
				if (FAILED(d3dResult))
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, "3D object index buffer creation failed (HRESULT %#010x)", d3dResult);
					eae6320::Logging::OutputError("Direct3D failed to create a 3D object index buffer (HRESULT %#010x)", d3dResult);
					return result;
				}
			}
			return result;
		}
		void DefaultGeometry::Draw()
		{
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);
			//Bind Index Buffer
			{
				EAE6320_ASSERT(m_indexBuffer);
				constexpr DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
				// The indices start at the beginning of the buffer
				constexpr unsigned int offset = 0;
				direct3dImmediateContext->IASetIndexBuffer(m_indexBuffer, indexFormat, offset);
			}
			// Bind a specific vertex buffer to the device as a data source
			{
				EAE6320_ASSERT(m_vertexBuffer);
				constexpr unsigned int startingSlot = 0;
				constexpr unsigned int vertexBufferCount = 1;
				// The "stride" defines how large a single vertex is in the stream of data
				constexpr unsigned int bufferStride = sizeof(VertexFormats::s3dObject);
				// It's possible to start streaming data in the middle of a vertex buffer
				constexpr unsigned int bufferOffset = 0;
				direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBuffer, &bufferStride, &bufferOffset);
			}
			// Specify what kind of data the vertex buffer holds
			{
				// Bind the vertex format (which defines how to interpret a single vertex)
				{
					EAE6320_ASSERT(m_vertexFormat);
					auto* const vertexFormat = cVertexFormat::s_manager.Get(m_vertexFormat);
					EAE6320_ASSERT(vertexFormat);
					if (vertexFormat)
						vertexFormat->Bind();
					else
						EAE6320_ASSERTF(false, "vertexFromat is nullptr");
				}
				// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
				// the vertex buffer was defined as a triangle list
				// (meaning that every primitive is a triangle and will be defined by three vertices)
				direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}

			//Render triangles from index buffer
			{
				constexpr unsigned int indexOfFirstIndexToUse = 0;
				constexpr unsigned int offsetToAddToEachIndex = 0;
				direct3dImmediateContext->DrawIndexed(static_cast<unsigned int>(indexCountToRender), indexOfFirstIndexToUse, offsetToAddToEachIndex);
			}
			//// Render triangles from the currently-bound vertex buffer
			//{
			//	// As of this comment only a single triangle is drawn
			//	// (you will have to update this code in future assignments!)
			//	//Change triangleCount to 2
			//	constexpr unsigned int triangleCount = 2;
			//	constexpr unsigned int vertexCountPerTriangle = 3;
			//	constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
			//	// It's possible to start rendering primitives in the middle of the stream
			//	constexpr unsigned int indexOfFirstVertexToRender = 0;
			//	direct3dImmediateContext->Draw(vertexCountToRender, indexOfFirstVertexToRender);
			//}
		}
	}
}
