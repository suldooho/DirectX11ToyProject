#pragma once
#include "TextureMesh.h"
#include "Vertices.h"

class FloorMesh : public TextureMesh
{
private:
	std::vector<TextureVertex> vertices_;

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual void* GetVertexData() override;

public:
	virtual void Initialize(std::string file_path);
};


	 