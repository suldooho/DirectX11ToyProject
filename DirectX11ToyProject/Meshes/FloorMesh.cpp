#include "FloorMesh.h"
#include "../framework.h"

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

    vertices_.emplace_back(TextureVertex(DirectX::XMFLOAT3(-10.0f, 0.0f, 10.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
    vertices_.emplace_back(TextureVertex(DirectX::XMFLOAT3(10.0f, 0.0f, 10.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
    vertices_.emplace_back(TextureVertex(DirectX::XMFLOAT3(-10.0f, 0.0f, -10.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
    vertices_.emplace_back(TextureVertex(DirectX::XMFLOAT3(10.0f, 0.0f, -10.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

    num_vertices_ = vertices_.size();
    stride_ = sizeof(TextureVertex);
    offset_ = 0;
}

void FloorMesh::CreateIndices()
{
}

unsigned int FloorMesh::GetVertexBufferByteWidth()
{
    return sizeof(TextureVertex) * vertices_.size();
}

void* FloorMesh::GetVertexData()
{
    return vertices_.data();
}

void FloorMesh::Initialize(std::string file_path)
{
    wchar_t current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);

    std::wstring w_obj_file_path;
    w_obj_file_path.assign(file_path.begin(), file_path.end());

    w_obj_file_path = std::wstring(current_path) + w_obj_file_path;
    file_path.assign(w_obj_file_path.begin(), w_obj_file_path.end());

    shader_resource_view_container_["DiffuseView"] = LoadTexture(file_path + "_Diffuse.png");
    shader_resource_view_container_["NormalView"] = LoadTexture(file_path + "_Normal.png");
    shader_resource_view_container_["HeightView"] = LoadTexture(file_path + "_Height.png");

    CreateFaceData();
}
