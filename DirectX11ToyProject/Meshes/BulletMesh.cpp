#include "BulletMesh.h"

void BulletMesh::CreateVertices()
{
    primitive_topology_ = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

}

void BulletMesh::CreateIndices()
{
}

void BulletMesh::CreateVertexBuffer()
{
}

void BulletMesh::CreateIndexBuffer()
{
}

void BulletMesh::Initialize(const std::string& file_name)
{
    Mesh<NullVertex>::Initialize();

    texture_component_ = std::make_unique<TextureComponent>();
    texture_component_->Initialize();
    texture_component_->LoadTexture(texture_component_->GetAbsolutePathPath(file_name) + "/Diffuse.png");
}
