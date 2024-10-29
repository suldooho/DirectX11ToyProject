#pragma once
#include "TextureMesh.h"
#include "Vertices.h"
#include <vector>
#include <wrl.h>

class SkySphereMesh : public TextureMesh
{
private:
	std::vector<SkySphereVertex> vertices_;

protected:
	virtual void CreateRasterizerState() override;

	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;  
	virtual void* GetVertexData() override; 

public:
	virtual void Initialize(std::string file_path) override;

private: 
	const int kRadius_ = 1000;
	const unsigned int kStackCount_ = 100;
	const unsigned int kSliceCount_ = 100;
};

