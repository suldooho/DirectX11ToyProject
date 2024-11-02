#pragma once
#include "Mesh.h"
#include "Vertices.h"

class EnemyMesh : public Mesh<TextureVertex>
{

protected:
	virtual void CreateVertices() = 0;
	virtual void CreateIndices() = 0;
};

