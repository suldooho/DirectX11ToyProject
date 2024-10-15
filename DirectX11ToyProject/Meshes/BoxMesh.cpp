#include "BoxMesh.h"
#include "../framework.h"

void BoxMesh::CreateVertices()
{
	vertices_[0] = { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices_[1] = { DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	vertices_[2] = { DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	vertices_[3] = { DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };
	vertices_[4] = { DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) };
	vertices_[5] = { DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	vertices_[6] = { DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
	vertices_[7] = { DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }; 

	num_vertices_ = 8;
	stride_ = sizeof(ColorVertex);
	offset_ = 0;
}

void BoxMesh::CreateIndices()
{
	indices_[0] = 0;
	indices_[1] = 1;
	indices_[2] = 2;
	indices_[3] = 0;
	indices_[4] = 2;
	indices_[5] = 3;
	indices_[6] = 4;
	indices_[7] = 6;
	indices_[8] = 5;
	indices_[9] = 4;
	indices_[10] = 7;
	indices_[11] = 6;
	indices_[12] = 4;
	indices_[13] = 5;
	indices_[14] = 1;
	indices_[15] = 4;
	indices_[16] = 1;
	indices_[17] = 0;
	indices_[18] = 3;
	indices_[19] = 2;
	indices_[20] = 6;
	indices_[21] = 3;
	indices_[22] = 6;
	indices_[23] = 7;
	indices_[24] = 1;
	indices_[25] = 5;
	indices_[26] = 6;
	indices_[27] = 1;
	indices_[28] = 6;
	indices_[29] = 2;
	indices_[30] = 4;
	indices_[31] = 0;
	indices_[32] = 3;
	indices_[33] = 4;
	indices_[34] = 3;
	indices_[35] = 7; 

	num_indices_ = 36;
	start_index_ = 0;
	base_vertex_ = 0;
}

unsigned int BoxMesh::GetVertexBufferByteWidth()
{
	return sizeof(ColorVertex) * 8;
}

unsigned int BoxMesh::GetIndexBufferByteWidth()
{
	return sizeof(unsigned int) * 36;
}

void* BoxMesh::GetVertexData()
{
	return vertices_;
}

void* BoxMesh::GetIndexData()
{
	return indices_;
}
