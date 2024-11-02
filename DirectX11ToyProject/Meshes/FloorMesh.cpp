#include "FloorMesh.h"
#include "../framework.h"
#include "TextureComponent.h"

void FloorMesh::CreateIndexBuffer()
{
}

void FloorMesh::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizer_desc;
    ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizer_desc.CullMode = D3D11_CULL_BACK;
    rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizer_desc.FrontCounterClockwise = false;
    DeviceManager::GetInstance()->GetD3D11Device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf());
}

void FloorMesh::CreateVertices()
{
	primitive_topology_ = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(-10.0f, 0.0f, 10.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(10.0f, 0.0f, 10.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(-10.0f, 0.0f, -10.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
    vertices_->emplace_back(TextureVertex(DirectX::XMFLOAT3(10.0f, 0.0f, -10.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

    num_vertices_ = vertices_->size();
    stride_ = sizeof(TextureVertex);
    offset_ = 0;
}

void FloorMesh::CreateIndices()
{
}

unsigned int FloorMesh::GetVertexBufferByteWidth()
{
    return sizeof(TextureVertex) * vertices_->size();
}

void* FloorMesh::GetVertexData()
{
    return vertices_->data();
}

void FloorMesh::Initialize(std::string file_name)
{
    vertices_ = std::make_unique<std::vector<TextureVertex>>();
    texture_component_ = std::make_unique<TextureComponent>();
     
    texture_component_->Initialize();
    std::string file_path = texture_component_->GetAbsolutePathPath(file_name);
    texture_component_->LoadTexture(file_path + "/Diffuse.png"); 
    texture_component_->LoadTexture(file_path + "/Normal.png");
    texture_component_->LoadTexture(file_path + "/Height.png"); 

    CreateFaceData();
}
