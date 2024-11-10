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
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
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
}

void FloorMesh::CreateIndices()
{
}

void FloorMesh::CreateTestRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizer_desc;
    ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizer_desc.CullMode = D3D11_CULL_BACK;
    rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizer_desc.FrontCounterClockwise = false;
    DeviceManager::GetInstance()->GetD3D11Device()->CreateRasterizerState(&rasterizer_desc, test_rasterizer_state_.GetAddressOf());
}


void FloorMesh::Initialize(const std::string& file_name)
{
    Mesh<TextureVertex>::Initialize();
    CreateTestRasterizerState();
    texture_component_ = std::make_unique<TextureComponent>(); 
    texture_component_->Initialize();
    std::string file_path = texture_component_->GetAbsolutePathPath(file_name);
    texture_component_->LoadTexture(file_path + "/Diffuse.png"); 
    texture_component_->LoadTexture(file_path + "/Normal.png");
    texture_component_->LoadTexture(file_path + "/Height.png");

}

ID3D11RasterizerState* FloorMesh::GetTestRasterizerState()
{
    return test_rasterizer_state_.Get();
}
