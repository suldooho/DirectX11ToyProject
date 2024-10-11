#pragma once
#include "Mesh.h"
#include "Vertices.h"

class BoxMesh : public Mesh
{ 
private:
	ColorVertex vertices_[8];
	unsigned int indices_[36];

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual unsigned int GetIndexBufferByteWidth() override;

	virtual void* GetVertexData() override;
	virtual void* GetIndexData() override;
};

