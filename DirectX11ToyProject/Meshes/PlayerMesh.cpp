#include "PlayerMesh.h"

void PlayerMesh::CreateVertices()
{
    vertices_ = obj_importer_component_->LoadVertices(obj_importer_component_->GetAbsolutePathPath() + "/Gun.txt");
}

void PlayerMesh::CreateIndices()
{
    indices_ = obj_importer_component_->LoadIndices(obj_importer_component_->GetAbsolutePathPath() + "/Index.txt");
} 
 
void PlayerMesh::Initialize(const std::string& file_name)
{
    obj_importer_component_ = std::make_unique<OBJImporterComponent>();
    obj_importer_component_->Initialize(file_name);
    Mesh<BumpMappingVertex>::Initialize();
     
    texture_component_ = std::make_unique<TextureComponent>();
    texture_component_->Initialize();
    texture_component_->LoadTexture(texture_component_->GetAbsolutePathPath(file_name) + "/Diffuse.png");
    texture_component_->LoadTexture(texture_component_->GetAbsolutePathPath(file_name) + "/Normal.png");

}
