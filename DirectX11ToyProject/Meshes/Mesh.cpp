#include "Mesh.h"
#include "../framework.h"

Mesh::~Mesh()
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
	DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, vertex_buffer_.GetAddressOf());
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
	DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, index_buffer_.GetAddressOf());
}

void Mesh::CreateRasterizerState()
{ 
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FrontCounterClockwise = false;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf()); 
}

unsigned int Mesh::GetIndexBufferByteWidth()
{
	return sizeof(unsigned int) * indices_->size();
}

void* Mesh::GetIndexData()
{
	return indices_->data();
}

void Mesh::CreateFaceData()
{
	CreateVertices();
	CreateIndices();

	CreateVertexBuffer();
	CreateIndexBuffer();

	CreateRasterizerState();
}

D3D_PRIMITIVE_TOPOLOGY Mesh::GetPrimitiveTopology() const
{
	return primitive_topology_;
}

unsigned int* Mesh::GetStride()
{
	return &stride_;
}

unsigned int* Mesh::GetOffset()
{
	return &offset_;
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
	return vertex_buffer_.Get();
}

unsigned int Mesh::GetNumIndices() const
{
	return num_indices_;
}

unsigned int Mesh::GetNumVertices() const
{
	return num_vertices_;
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
	return index_buffer_.Get();
}

ID3D11RasterizerState* Mesh::GetRasterizerState()
{
	return rasterizer_state_.Get();
}
