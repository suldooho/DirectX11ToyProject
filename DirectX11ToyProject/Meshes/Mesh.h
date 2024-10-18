#pragma once
#include <wrl.h>
#include <d3d11.h>

class Mesh
{
protected:
	enum D3D_PRIMITIVE_TOPOLOGY primitive_topology_;

	unsigned int num_vertices_;
	unsigned int stride_;
	unsigned int offset_;

	Microsoft::WRL::ComPtr<ID3D11Buffer> d3d11_vertex_buffer_;
	
protected: 
	unsigned int num_indices_;
	unsigned int start_index_;
	int base_vertex_;

	Microsoft::WRL::ComPtr<ID3D11Buffer> d3d11_index_buffer_;
	  
protected:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;

private:
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateSolidRasterizerState();

protected:
	virtual void CreateVertices() = 0;
	virtual void CreateIndices() = 0;

	virtual unsigned int GetVertexBufferByteWidth() = 0;
	virtual unsigned int GetIndexBufferByteWidth() = 0;

	virtual void* GetVertexData() = 0;
	virtual void* GetIndexData() = 0;

public:
	void Initialize();

public:
	enum D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
	unsigned int GetStride() const;
	unsigned int GetOffset() const;
	ID3D11Buffer* GetVertexBuffer() const;
	unsigned int GetNumIndices() const;
	unsigned int GetStartIndex() const;
	int GetBaseVertex() const;
	ID3D11Buffer* GetIndexBuffer() const;
	ID3D11RasterizerState* GetRasterizerState();
};

