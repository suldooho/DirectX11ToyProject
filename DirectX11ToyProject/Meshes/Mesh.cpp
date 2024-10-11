#include "Mesh.h"
#include "../framework.h"

Mesh::Mesh() : num_vertices_(0), stride_(0), offset_(0), d3d11_vertex_buffer_(nullptr), primitive_topology_(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), num_indices_(0), start_index_(0), base_vertex_(0), d3d11_index_buffer_(nullptr)
{ 
}

void Mesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buffer_desc.ByteWidth = GetVertexBufferByteWidth();
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = GetVertexData();
	Device::GetInstace()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, &d3d11_vertex_buffer_);
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
	Device::GetInstace()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, &d3d11_index_buffer_);
}

void Mesh::Initialize()
{
	CreateVertices();
	CreateIndices();

	CreateVertexBuffer();
	CreateIndexBuffer();
}
