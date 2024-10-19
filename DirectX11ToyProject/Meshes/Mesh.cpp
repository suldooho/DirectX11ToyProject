#include "Mesh.h"
#include "../framework.h"

void Mesh::CreateVertexBuffer()
{
	primitive_topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.ByteWidth = GetVertexBufferByteWidth();
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = GetVertexData();
	DeviceManager::GetInstace()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, d3d11_vertex_buffer_.GetAddressOf());
}

void Mesh::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.ByteWidth = GetIndexBufferByteWidth();
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = GetIndexData();
	DeviceManager::GetInstace()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, d3d11_index_buffer_.GetAddressOf());
}

void Mesh::CreateSolidRasterizerState()
{ 
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	DeviceManager::GetInstace()->GetD3D11Device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf()); 
}

void Mesh::Initialize()
{
	CreateVertices();
	CreateIndices();

	CreateVertexBuffer();
	CreateIndexBuffer();

	CreateSolidRasterizerState();
}

D3D_PRIMITIVE_TOPOLOGY Mesh::GetPrimitiveTopology() const
{
	return primitive_topology_;
}

unsigned int Mesh::GetStride() const
{
	return stride_;
}

unsigned int Mesh::GetOffset() const
{
	return offset_;
}

ID3D11Buffer* Mesh::GetVertexBuffer() const
{
	return d3d11_vertex_buffer_.Get();
}

unsigned int Mesh::GetNumIndices() const
{
	return num_indices_;
}

unsigned int Mesh::GetStartIndex() const
{
	return start_index_;
}

int Mesh::GetBaseVertex() const
{
	return base_vertex_;
}

ID3D11Buffer* Mesh::GetIndexBuffer() const
{
	return d3d11_index_buffer_.Get();
}

ID3D11RasterizerState* Mesh::GetRasterizerState()
{
	return rasterizer_state_.Get();
}
