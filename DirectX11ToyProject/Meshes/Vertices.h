#pragma once
#include <DirectXMath.h>

struct ColorVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;

	ColorVertex(DirectX::XMFLOAT3 newPosition, DirectX::XMFLOAT4 newColor)
	{
		position = newPosition;
		color = newColor;
	}
};

struct TextureVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord; 

	TextureVertex(DirectX::XMFLOAT3 newPosition, DirectX::XMFLOAT2 newTexcoord)
	{
		position = newPosition;
		texcoord = newTexcoord;
	}
};

struct BumpMappingVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
	DirectX::XMFLOAT3 normal; 

	BumpMappingVertex()
	{ 
	}
	 
	BumpMappingVertex(DirectX::XMFLOAT3 newPosition, DirectX::XMFLOAT2 newTexcoord, DirectX::XMFLOAT3 newTangent, DirectX::XMFLOAT3 newBitangent, DirectX::XMFLOAT3 newNormal)
	{
		position = newPosition;
		texcoord = newTexcoord;
		tangent = newTangent;
		bitangent = newBitangent;
		normal = newNormal;
	}
};
 
