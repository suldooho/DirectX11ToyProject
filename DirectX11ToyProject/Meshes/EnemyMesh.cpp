#include "EnemyMesh.h"
 
void EnemyMesh::CreateVertices()
{
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));  
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f))); 
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f))); 
}

void EnemyMesh::CreateIndices()
{
    indices_->emplace_back(0); 
    indices_->emplace_back(1);
    indices_->emplace_back(2);

    indices_->emplace_back(0);
    indices_->emplace_back(2);
    indices_->emplace_back(3);

}

void EnemyMesh::Initialize(const std::string& file_name)
{
    Mesh<TextureVertex>::Initialize();

    texture_component_ = std::make_unique<TextureComponent>();
    texture_component_->Initialize();
    texture_component_->LoadTexture(texture_component_->GetAbsolutePathPath(file_name) + "/Diffuse.png"); 
}
