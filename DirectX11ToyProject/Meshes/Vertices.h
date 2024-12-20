#pragma once
#include <DirectXMath.h>

struct NullVertex
{

};

struct ColorVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;

	ColorVertex(DirectX::XMFLOAT3 new_position, DirectX::XMFLOAT3 new_color)
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
 
struct PositionInstanceData 
{
	DirectX::XMFLOAT3 position;
};

struct EnemyInstanceData
{
	DirectX::XMFLOAT4 world0;
	DirectX::XMFLOAT4 world1;
	DirectX::XMFLOAT4 world2;
	DirectX::XMFLOAT4 world3;
	DirectX::XMFLOAT3 color;
};

struct BulletInstanceData
{ 
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 prevPosition;
	DirectX::XMFLOAT3 color;
};