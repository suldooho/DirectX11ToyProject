#pragma once
#include "Mesh.h"
#include "Vertices.h"
#include "TextureComponent.h"

class BulletMesh : public Mesh<NullVertex>
{
public:
	std::unique_ptr<TextureComponent> texture_component_;

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual void CreateVertexBuffer() override;
	virtual void CreateIndexBuffer() override;

public:
	virtual void Initialize(const std::string& file_name) override;
};

