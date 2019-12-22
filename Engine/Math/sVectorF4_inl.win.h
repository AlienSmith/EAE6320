#pragma once
namespace DXMath {
	inline float sVectorF4::dot(const sVectorF4& other)
	{
		return DirectX::XMVectorGetX(DirectX::XMVector4Dot(data, other.data));
	}
}