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

void Mesh::Initialize()
{
	CreateVertices();
	CreateIndices();

	CreateVertexBuffer();
	CreateIndexBuffer();
}
