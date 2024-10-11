#pragma once
#include <wrl.h> 

class Mesh
{
protected:
	unsigned int num_vertices_;
	unsigned int stride_;
	unsigned int offset_;

	Microsoft::WRL::ComPtr<class ID3D11Buffer> d3d11_vertex_buffer_;
	enum D3D_PRIMITIVE_TOPOLOGY primitive_topology_;
	
protected: 
	unsigned int num_indices_;
	unsigned int start_index_;
	int base_vertex_;

	Microsoft::WRL::ComPtr<class ID3D11Buffer> d3d11_index_buffer_;

public:
	Mesh();

private:
	void CreateVertexBuffer();
	void CreateIndexBuffer();

protected:
	virtual void CreateVertices() = 0;
	virtual void CreateIndices() = 0;

	virtual unsigned int GetVertexBufferByteWidth() = 0;
	virtual unsigned int GetIndexBufferByteWidth() = 0;

	virtual void* GetVertexData() = 0;
	virtual void* GetIndexData() = 0;

public:
	void Initialize();
};

