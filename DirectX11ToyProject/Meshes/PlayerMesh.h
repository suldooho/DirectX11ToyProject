#pragma once
#include "Mesh.h"
#include "Vertices.h" 
#include "OBJImporterComponent.h"
#include "TextureComponent.h"
#include <vector>
#include <string>
#include <memory>

class PlayerMesh : public Mesh
{
private:
	std::unique_ptr<std::vector<BumpMappingVertex>> vertices_;

public: // Component´Â public
	std::unique_ptr<OBJImporterComponent> obj_importer_component_;
	std::unique_ptr<TextureComponent> texture_component_;

protected: 
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;
	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual void* GetVertexData() override;

public:
	void Initialize(std::string file_path);
};

