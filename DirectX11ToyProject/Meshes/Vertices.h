#pragma once
#include <DirectXMath.h>

struct ColorVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

struct BumpMappingVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	DirectX::XMFLOAT3 normal; 
};
 