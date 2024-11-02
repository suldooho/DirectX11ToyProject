#pragma once
#include <DirectXMath.h>

struct ColorVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;

	ColorVertex(DirectX::XMFLOAT3 new_position, DirectX::XMFLOAT4 new_color)
	{
		position = new_position;
		color = new_color;
	}
};

struct TextureVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord; 

	TextureVertex(DirectX::XMFLOAT3 new_position, DirectX::XMFLOAT2 new_texcoord)
	{
		position = new_position;
		texcoord = new_texcoord;
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
	 
	BumpMappingVertex(DirectX::XMFLOAT3 new_position, DirectX::XMFLOAT2 new_texcoord, DirectX::XMFLOAT3 new_tangent, DirectX::XMFLOAT3 new_bitangent, DirectX::XMFLOAT3 new_normal)
	{
		position = new_position;
		texcoord = new_texcoord;
		tangent = new_tangent;
		bitangent = new_bitangent;
		normal = new_normal;
	}
};
 
struct WorldInstanceData 
{
	DirectX::XMFLOAT4X4 world_matrix;
};