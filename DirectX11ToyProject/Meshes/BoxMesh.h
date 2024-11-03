#pragma once
#include "Mesh.h"
#include "Vertices.h"
#include <vector>
#include <memory>

class BoxMesh : public Mesh<ColorVertex>
{  
protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override; 

public: 
	virtual void Initialize(const std::string& file_name) override;
};

