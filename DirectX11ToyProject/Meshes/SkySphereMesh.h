#pragma once
#include "Mesh.h"
#include "Vertices.h"   
#include "TextureComponent.h"
#include <vector>
#include <memory>
#include <string>

class SkySphereMesh : public Mesh
{
private:
	std::unique_ptr<std::vector<TextureVertex>> vertices_;

public: // Component┤┬ public
	std::unique_ptr<TextureComponent> texture_component_;

protected:
	virtual void CreateRasterizerState() override;

	virtual void CreateVertices() override;
	virtual void CreateIndices() override;
	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual void* GetVertexData() override; 

public:
	void Initialize(std::string file_path);

private: 
	const int kRadius_ = 1000;
	const unsigned int kStackCount_ = 100;
	const unsigned int kSliceCount_ = 100;
};

