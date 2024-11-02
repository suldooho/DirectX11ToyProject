#include "BoxMesh.h"
#include "../framework.h"

void BoxMesh::CreateVertices()
{
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f)));
	vertices_->emplace_back(ColorVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f)));
}

void BoxMesh::CreateIndices()
{
	indices_->emplace_back(0);
	indices_->emplace_back(1);
	indices_->emplace_back(2);
	indices_->emplace_back(0);
	indices_->emplace_back(2);
	indices_->emplace_back(3);
	indices_->emplace_back(4);
	indices_->emplace_back(6);
	indices_->emplace_back(5);
	indices_->emplace_back(4);
	indices_->emplace_back(7);
	indices_->emplace_back(6);
	indices_->emplace_back(4);
	indices_->emplace_back(0);
	indices_->emplace_back(3);
	indices_->emplace_back(4);
	indices_->emplace_back(3);
	indices_->emplace_back(7);
	indices_->emplace_back(1);
	indices_->emplace_back(5);
	indices_->emplace_back(6);
	indices_->emplace_back(1);
	indices_->emplace_back(6);
	indices_->emplace_back(2);
	indices_->emplace_back(4);
	indices_->emplace_back(5);
	indices_->emplace_back(1);
	indices_->emplace_back(4);
	indices_->emplace_back(1);
	indices_->emplace_back(0);
	indices_->emplace_back(3);
	indices_->emplace_back(2);
	indices_->emplace_back(6);
	indices_->emplace_back(3);
	indices_->emplace_back(6);
	indices_->emplace_back(7); 
}

void BoxMesh::Initialize(std::string file_name)
{
	Mesh<ColorVertex>::Initialize();
} 
