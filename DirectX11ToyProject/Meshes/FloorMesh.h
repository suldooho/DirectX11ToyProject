#pragma once
#include "Mesh.h"
#include "Vertices.h" 
#include "TextureComponent.h"
#include <vector>
#include <string>
#include <memory>

class FloorMesh : public Mesh
{
private:
	std::unique_ptr<std::vector<TextureVertex>> vertices_;

public: // Component´Â public
	std::unique_ptr<TextureComponent> texture_component_;

protected:
	virtual void CreateIndexBuffer() override;

	virtual void CreateRasterizerState() override;

	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual void* GetVertexData() override;

public:
	void Initialize(std::string file_name);
};


	 