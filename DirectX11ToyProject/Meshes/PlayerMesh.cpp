#include "PlayerMesh.h"

void PlayerMesh::CreateVertices()
{
    num_vertices_ = vertices_->size();
    stride_ = sizeof(BumpMappingVertex);
    offset_ = 0;
}

void PlayerMesh::CreateIndices()
{
    num_indices_ = indices_->size();
    start_index_ = 0;
    base_vertex_ = 0;
}

unsigned int PlayerMesh::GetVertexBufferByteWidth()
{
    return sizeof(BumpMappingVertex) * vertices_->size();
}

void* PlayerMesh::GetVertexData()
{
    return vertices_->data();
}

void PlayerMesh::Initialize(std::string file_name)
{
    obj_importer_component_ = std::make_unique<OBJImporterComponent>();
    texture_component_ = std::make_unique<TextureComponent>();
    
    std::string file_path = obj_importer_component_->GetAbsolutePathPath(file_name);
    vertices_ = obj_importer_component_->LoadVertices(file_path + "/Gun.txt");
    indices_ = obj_importer_component_->LoadIndices(file_path + "/Index.txt");

    texture_component_->Initialize();
    texture_component_->LoadTexture(file_path + "/Diffuse.png");
    texture_component_->LoadTexture(file_path + "/Normal.png");

    CreateFaceData();
}
