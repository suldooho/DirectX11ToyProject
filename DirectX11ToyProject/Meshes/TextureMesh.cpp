#include "TextureMesh.h"
#include "../framework.h"

ID3D11ShaderResourceView* TextureMesh::LoadTexture(std::string file_path)
{
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImg;

    std::wstring w_obj_texture_file_path;
    w_obj_texture_file_path.assign(file_path.begin(), file_path.end());

    CoInitialize(NULL);
    HRESULT result = DirectX::LoadFromWICFile(w_obj_texture_file_path.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);

    if (result != S_OK)
    {
        throw std::string("Can Not Load " + file_path);
    }

    ID3D11ShaderResourceView* texture_view;
    result = DirectX::CreateShaderResourceView(DeviceManager::GetInstance()->GetD3D11Device(), scratchImg.GetImages(), scratchImg.GetImageCount(), metadata, &texture_view);

    if (result != S_OK)
    {
        throw std::string("Can Not Create Texture View");
    }

    return texture_view;
}

void TextureMesh::CreateSamplerState()
{
    D3D11_SAMPLER_DESC sampler_desc;
    ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));
    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

    DeviceManager::GetInstance()->GetD3D11Device()->CreateSamplerState(&sampler_desc, sampler_state_.GetAddressOf());
} 

ID3D11ShaderResourceView** TextureMesh::GetTextureShaderResourceView(std::string texture_name)
{ 
    if (shader_resource_view_container_.find(texture_name) != shader_resource_view_container_.end())
    {
        return shader_resource_view_container_[texture_name].GetAddressOf();
    }
    
    throw std::string("Texture Resource View Name Error"); 
}

ID3D11SamplerState** TextureMesh::GetSampler()
{
    return sampler_state_.GetAddressOf();
} 
