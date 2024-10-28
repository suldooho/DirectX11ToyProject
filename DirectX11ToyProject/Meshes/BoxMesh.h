#pragma once
#include "Mesh.h"
#include "Vertices.h"
#include <vector>

class BoxMesh : public Mesh
{
private:
	std::vector<ColorVertex> vertices_;

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;  
	virtual void* GetVertexData() override; 

public:
	void Initialize();
};

