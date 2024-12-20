#pragma once
#include "Mesh.h"
#include "Vertices.h" 
#include "OBJImporterComponent.h"
#include "TextureComponent.h"
#include <vector>
#include <string>
#include <memory>

class PlayerMesh : public Mesh<BumpMappingVertex>
{  
public: // Component┤┬ public
	std::unique_ptr<OBJImporterComponent> obj_importer_component_;
	std::unique_ptr<TextureComponent> texture_component_;

protected: 
	virtual void CreateVertices() override;
	virtual void CreateIndices() override; 
	virtual void CreateRasterizerState() override;
public:
	virtual void Initialize(const std::string& file_name) override;
};

