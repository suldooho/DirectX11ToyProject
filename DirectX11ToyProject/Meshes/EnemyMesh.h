#pragma once
#include "Mesh.h"
#include "Vertices.h"
#include <memory>
#include "TextureComponent.h"

class EnemyMesh : public Mesh<TextureVertex>
{
public:
	std::unique_ptr<TextureComponent> texture_component_;

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

public:
	virtual void Initialize(const std::string& file_name) override;
};

