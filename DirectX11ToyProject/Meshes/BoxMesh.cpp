#include "BoxMesh.h"
#include "../framework.h"

void BoxMesh::CreateVertices()
{
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));
	vertices_.emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)));

	num_vertices_ = vertices_.size();
	stride_ = sizeof(ColorVertex);
	offset_ = 0;
}

void BoxMesh::CreateIndices()
{
	indices_.emplace_back(0);
	indices_.emplace_back(1);
	indices_.emplace_back(2);
	indices_.emplace_back(0);
	indices_.emplace_back(2);
	indices_.emplace_back(3);
	indices_.emplace_back(4);
	indices_.emplace_back(6);
	indices_.emplace_back(5);
	indices_.emplace_back(4);
	indices_.emplace_back(7);
	indices_.emplace_back(6);
	indices_.emplace_back(4);
	indices_.emplace_back(0);
	indices_.emplace_back(3);
	indices_.emplace_back(4);
	indices_.emplace_back(3);
	indices_.emplace_back(7);
	indices_.emplace_back(1);
	indices_.emplace_back(5);
	indices_.emplace_back(6);
	indices_.emplace_back(1);
	indices_.emplace_back(6);
	indices_.emplace_back(2);
	indices_.emplace_back(4);
	indices_.emplace_back(5);
	indices_.emplace_back(1);
	indices_.emplace_back(4);
	indices_.emplace_back(1);
	indices_.emplace_back(0);
	indices_.emplace_back(3);
	indices_.emplace_back(2);
	indices_.emplace_back(6);
	indices_.emplace_back(3);
	indices_.emplace_back(6);
	indices_.emplace_back(7); 

	num_indices_ = indices_.size();
	start_index_ = 0;
	base_vertex_ = 0;
}

unsigned int BoxMesh::GetVertexBufferByteWidth()
{
	return sizeof(ColorVertex) * vertices_.size();
} 

void* BoxMesh::GetVertexData()
{
	return vertices_.data();
} 

void BoxMesh::Initialize()
{
	CreateFaceData();
}
