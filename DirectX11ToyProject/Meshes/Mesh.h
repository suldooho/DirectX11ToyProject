#pragma once
#include "MeshBase.h"
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <memory>
#include <string>
#include "../Core/DeviceManager.h"

template <typename T>
class Mesh : public MeshBase
{ 
protected:
	D3D_PRIMITIVE_TOPOLOGY primitive_topology_;
	 
	std::unique_ptr<std::vector<T>> vertices_;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer_;
	
protected:
	std::unique_ptr<std::vector<unsigned int>> indices_;
	 
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer_;
	  
protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;

public:
	~Mesh();

private:
	void CreateVertexBuffer();
	virtual void CreateIndexBuffer();

protected:
	virtual void CreateRasterizerState();

	virtual void CreateVertices() = 0;
	virtual void CreateIndices() = 0;

	virtual unsigned int GetVertexBufferByteWidth();
	unsigned int GetIndexBufferByteWidth();

	virtual void* GetVertexData();
	void* GetIndexData();
	 
	void Initialize();

public:
	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
	unsigned int GetStride();
	unsigned int GetOffset();
	ID3D11Buffer* GetVertexBuffer();
	unsigned int GetNumIndices() const;
	unsigned int GetNumVertices() const;
	unsigned int GetStartIndex() const;
	int GetBaseVertex() const;
	ID3D11Buffer* GetIndexBuffer() const;
	ID3D11RasterizerState* GetRasterizerState();

	virtual void Initialize(std::string file_name) = 0;
};

template<typename T>
Mesh<T>::~Mesh()
{
}

template<typename T>
void Mesh<T>::CreateVertexBuffer()
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

template<typename T>
void Mesh<T>::CreateIndexBuffer()
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

template<typename T>
void Mesh<T>::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FrontCounterClockwise = false;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf());
}

template<typename T>
unsigned int Mesh<T>::GetVertexBufferByteWidth()
{
	return sizeof(T) * vertices_->size();
}

template<typename T>
unsigned int Mesh<T>::GetIndexBufferByteWidth()
{
	return sizeof(unsigned int) * indices_->size();
}

template<typename T>
void* Mesh<T>::GetVertexData()
{
	return vertices_->data();
}

template<typename T>
void* Mesh<T>::GetIndexData()
{
	return indices_->data();
}

template<typename T>
void Mesh<T>::Initialize()
{
	vertices_ = std::make_unique<std::vector<T>>();
	indices_ = std::make_unique<std::vector<unsigned int>>();

	CreateVertices();
	CreateIndices();

	CreateVertexBuffer();
	CreateIndexBuffer();

	CreateRasterizerState();
}

template<typename T>
D3D_PRIMITIVE_TOPOLOGY Mesh<T>::GetPrimitiveTopology() const
{
	return primitive_topology_;
}

template<typename T>
unsigned int Mesh<T>::GetStride()
{
	return sizeof(T);
}

template<typename T>
unsigned int Mesh<T>::GetOffset()
{
	return 0;
}

template<typename T>
ID3D11Buffer* Mesh<T>::GetVertexBuffer()
{
	return vertex_buffer_.Get();
}

template<typename T>
unsigned int Mesh<T>::GetNumIndices() const
{
	return indices_->size();
}

template<typename T>
unsigned int Mesh<T>::GetNumVertices() const
{
	return vertices_->size();
}

template<typename T>
unsigned int Mesh<T>::GetStartIndex() const
{
	return 0;
}

template<typename T>
int Mesh<T>::GetBaseVertex() const
{
	return 0;
}

template<typename T>
ID3D11Buffer* Mesh<T>::GetIndexBuffer() const
{
	return index_buffer_.Get();
}

template<typename T>
ID3D11RasterizerState* Mesh<T>::GetRasterizerState()
{
	return rasterizer_state_.Get();
}
