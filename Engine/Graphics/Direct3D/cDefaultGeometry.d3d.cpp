#include "cDefaultGeometry.d3d.h"
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
namespace eae6320 {
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
			return result;
		}
		eae6320::cResult DefaultGeometry::InitializeGeometry() {
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
			{
				constexpr unsigned int triangleCount = 1;
				constexpr unsigned int vertexCountPerTriangle = 3;
				constexpr auto vertexCount = triangleCount * vertexCountPerTriangle;
				eae6320::Graphics::VertexFormats::s3dObject vertexData[vertexCount];
				{
					vertexData[0].x = 0.0f;
					vertexData[0].y = 0.0f;
					vertexData[0].z = 0.0f;

					vertexData[1].x = 1.0f;
					vertexData[1].y = 1.0f;
					vertexData[1].z = 0.0f;

					vertexData[2].x = 1.0f;
					vertexData[2].y = 0.0f;
					vertexData[2].z = 0.0f;
				}
				D3D11_BUFFER_DESC bufferDescription{};
				{
					const auto bufferSize = vertexCount * sizeof(eae6320::Graphics::VertexFormats::s3dObject);
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
					initialData.pSysMem = vertexData;
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

			return result;
		}
		DefaultGeometry::~DefaultGeometry()
		{
			CleanUp();
		}
}
