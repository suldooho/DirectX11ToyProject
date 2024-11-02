#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <memory>

class Mesh
{ 
protected:
	enum D3D_PRIMITIVE_TOPOLOGY primitive_topology_;

	unsigned int num_vertices_;
	unsigned int stride_;
	unsigned int offset_;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer_;
	
protected:
	std::unique_ptr<std::vector<unsigned int>> indices_;

	unsigned int num_indices_;
	unsigned int start_index_;
	int base_vertex_;

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

	virtual unsigned int GetVertexBufferByteWidth() = 0;
	unsigned int GetIndexBufferByteWidth();

	virtual void* GetVertexData() = 0;
	void* GetIndexData();
	 
	void CreateFaceData();

public:
	enum D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
	unsigned int* GetStride();
	unsigned int* GetOffset();
	ID3D11Buffer** GetVertexBuffer();
	unsigned int GetNumIndices() const;
	unsigned int GetNumVertices() const;
	unsigned int GetStartIndex() const;
	int GetBaseVertex() const;
	ID3D11Buffer* GetIndexBuffer() const;
	ID3D11RasterizerState* GetRasterizerState();
};

