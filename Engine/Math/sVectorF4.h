#pragma once
#if defined(_WIN32) || defined(WIN32)
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#else 
struct FLOAT4
{
	float f[4];
};
#endif
namespace DXMath {
	class sVectorF4 {
#if defined(_WIN32) || defined(WIN32)
	private:
		DirectX::XMVECTORF32 data;
#else
	private:
		FLOAT4 data;
#endif
	public:
		sVectorF4(float x, float y, float z, float w){
			data.f[0] = x;
			data.f[1] = y;
			data.f[2] = z;
			data.f[3] = w;
		}
		inline float* operator[](int index) {
			return reinterpret_cast<float*>(&data) + index;
		}
		inline float operator[](int index) const {
			return *(reinterpret_cast<const float*>(&data) + index);
		}
		inline float dot(const sVectorF4& other);
#if defined(_WIN32) || defined(WIN32)
#endif
	};
}
#include "sVectorF4_inl.win.h"