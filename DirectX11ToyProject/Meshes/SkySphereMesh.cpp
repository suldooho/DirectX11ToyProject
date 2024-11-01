#include "SkySphereMesh.h"
#include "../framework.h"

void SkySphereMesh::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.FrontCounterClockwise = false;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateRasterizerState(&rasterizer_desc, rasterizer_state_.GetAddressOf());
}

void SkySphereMesh::CreateVertices()
{
    primitive_topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    vertices_.emplace_back(TextureVertex(DirectX::XMFLOAT3(0.0f, kRadius_, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
     
    float phi_step = DirectX::XM_PI / kStackCount_;
    float theta_step = 2.0f * DirectX::XM_PI / kSliceCount_;
     
    for (unsigned int i = 1; i <= kStackCount_ - 1; ++i)
    {
        float phi = i * phi_step;

        for (unsigned int j = 0; j <= kSliceCount_; ++j)
        {
            float theta = j * theta_step;

            DirectX::XMFLOAT3 pos(kRadius_ * sinf(phi) * cosf(theta), kRadius_ * cosf(phi), kRadius_ * sinf(phi) * sinf(theta));
            DirectX::XMFLOAT2 texcoord(theta / (DirectX::XM_2PI), phi / DirectX::XM_PI); 

            vertices_.emplace_back(pos, texcoord);
        }
    }
     
    vertices_.emplace_back(TextureVertex(DirectX::XMFLOAT3(0.0f, -kRadius_, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

    num_vertices_ = vertices_.size();
    stride_ = sizeof(TextureVertex);
    offset_ = 0;
}

void SkySphereMesh::CreateIndices()
{ 
    for (unsigned int i = 1; i <= kSliceCount_; ++i)
    {
        indices_.emplace_back(0);
        indices_.emplace_back(i + 1);
        indices_.emplace_back(i);
    }

    int base_index = 1;
    int ring_vertex_count = kSliceCount_ + 1;
    for (unsigned int i = 0; i < kStackCount_ - 2; ++i)
    {
        for (unsigned int j = 0; j < kSliceCount_; ++j)
        {
            indices_.emplace_back(base_index + i * ring_vertex_count + j);
            indices_.emplace_back(base_index + i * ring_vertex_count + j + 1);
            indices_.emplace_back(base_index + (i + 1) * ring_vertex_count + j);
                    
            indices_.emplace_back(base_index + (i + 1) * ring_vertex_count + j);
            indices_.emplace_back(base_index + i * ring_vertex_count + j + 1);
            indices_.emplace_back(base_index + (i + 1) * ring_vertex_count + j + 1);
        }
    }

    // Bottom stack
    unsigned int south_pole_index = (unsigned int)vertices_.size() - 1;
    base_index = south_pole_index - ring_vertex_count;
    for (int i = 0; i < kSliceCount_; ++i)
    {
        indices_.emplace_back(south_pole_index);
        indices_.emplace_back(base_index + i);
        indices_.emplace_back(base_index + i + 1);
    }

    num_indices_ = indices_.size();
    start_index_ = 0;
    base_vertex_ = 0;
}

unsigned int SkySphereMesh::GetVertexBufferByteWidth()
{
    return sizeof(TextureVertex) * vertices_.size();
} 

void* SkySphereMesh::GetVertexData()
{
    return vertices_.data();
}

void SkySphereMesh::Initialize(std::string file_path)
{
    wchar_t current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);

    std::wstring w_obj_file_path;
    w_obj_file_path.assign(file_path.begin(), file_path.end());

    w_obj_file_path = std::wstring(current_path) + w_obj_file_path;
    file_path.assign(w_obj_file_path.begin(), w_obj_file_path.end());
      
    shader_resource_view_container_["DiffuseView"] = LoadTexture(file_path + "_Diffuse.png");
    CreateSamplerState();

    CreateFaceData();
}
 
