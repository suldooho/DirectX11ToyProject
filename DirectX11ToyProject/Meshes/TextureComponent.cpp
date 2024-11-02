#include "TextureComponent.h"
#include "../framework.h"  
#include <iostream>

void TextureComponent::LoadTexture(std::string texture_file_path)
{
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImg;

    std::wstring w_obj_texture_file_path;
    w_obj_texture_file_path.assign(texture_file_path.begin(), texture_file_path.end());

    CoInitialize(NULL);
    HRESULT result = DirectX::LoadFromWICFile(w_obj_texture_file_path.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);

    if (result != S_OK)
    {
        throw std::string("Can Not Load " + texture_file_path);
    }

    ID3D11ShaderResourceView* texture_view;
    result = DirectX::CreateShaderResourceView(DeviceManager::GetInstance()->GetD3D11Device(), scratchImg.GetImages(), scratchImg.GetImageCount(), metadata, &texture_view);

    if (result != S_OK)
    {
        throw std::string("Can Not Create Texture View");
    }

    size_t last_slash_pos = texture_file_path.find_last_of("/\\");
    std::string filename_with_extension = texture_file_path.substr(last_slash_pos + 1);
     
    size_t dot_pos = filename_with_extension.find_last_of(".");
    std::string filename = filename_with_extension.substr(0, dot_pos);
     
    std::string new_name = filename + "View";

    shader_resource_view_container_[new_name] = texture_view;
}

std::string TextureComponent::GetAbsolutePathPath(std::string file_path)
{
    wchar_t current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);
    std::wstring w_obj_file_path;
    w_obj_file_path.assign(file_path.begin(), file_path.end());
    w_obj_file_path = std::wstring(current_path) + w_obj_file_path;
    file_path.assign(w_obj_file_path.begin(), w_obj_file_path.end());

    return file_path;
}

void TextureComponent::Initialize()
{
    CreateSamplerState();
}

void TextureComponent::CreateSamplerState()
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

ID3D11ShaderResourceView** TextureComponent::GetTextureShaderResourceView(std::string texture_name)
{ 
    if (shader_resource_view_container_.find(texture_name) != shader_resource_view_container_.end())
    {
        return shader_resource_view_container_[texture_name].GetAddressOf();
    }
    
    throw std::string("Texture Resource View Name Error"); 
}

ID3D11SamplerState** TextureComponent::GetSampler()
{
    return sampler_state_.GetAddressOf();
} 
