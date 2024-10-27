#include "LightsManager.h"
#include "../framework.h"

LightsManager* LightsManager::instance_ = nullptr;

void LightsManager::Initialize()
{
    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.ByteWidth = sizeof(PointLight) * kMaxPointLightNum_;
    buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    buffer_desc.StructureByteStride = sizeof(PointLight);
    ID3D11Buffer* point_light_buffer = nullptr;
    HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &point_light_buffer);
    if (result != S_OK)
    {
        throw std::string("Failed To Create Point Light Buffer");
    }
    d3d11_light_buffer_container_["PointLightBuffer"] = point_light_buffer;
     
    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
    ZeroMemory(&shader_resource_view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shader_resource_view_desc.Format = DXGI_FORMAT_UNKNOWN;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    shader_resource_view_desc.Buffer.FirstElement = 0;
    shader_resource_view_desc.Buffer.NumElements = kMaxPointLightNum_;
    ID3D11ShaderResourceView* point_light_shader_resource_view = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateShaderResourceView(d3d11_light_buffer_container_["PointLightBuffer"].Get(), &shader_resource_view_desc, &point_light_shader_resource_view);
    if (result != S_OK)
    {
        throw std::string("Failed To Create Point Light SRV");
    }
    d3d11_shader_resource_view_container_["PointLightShaderResourceView"] = point_light_shader_resource_view;

     
    buffer_desc.ByteWidth = sizeof(SpotLight) * kMaxSpotLightNum_;
    buffer_desc.StructureByteStride = sizeof(SpotLight); 
    ID3D11Buffer* spot_light_buffer = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &spot_light_buffer); 
    if (result != S_OK)
    {
        throw std::string("Failed To Create Spot Light Buffer");
    }
    d3d11_light_buffer_container_["SpotLightBuffer"] = spot_light_buffer;

    shader_resource_view_desc.Buffer.NumElements = kMaxSpotLightNum_;
    ID3D11ShaderResourceView* spot_light_shader_resource_view = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateShaderResourceView(d3d11_light_buffer_container_["SpotLightBuffer"].Get(), &shader_resource_view_desc, &spot_light_shader_resource_view);
    if (result != S_OK)
    {
        throw std::string("Failed to create Spot Light SRV");
    }
    d3d11_shader_resource_view_container_["SpotLightShaderResourceView"] = spot_light_shader_resource_view; 
}

void LightsManager::UpdateLightBuffers()
{
    ID3D11DeviceContext* context = DeviceManager::GetInstance()->GetD3D11ImmediateContext();

    // 조명 데이터 생성 (예시로 더미 데이터 설정)
    std::vector<PointLight> pointLights(1);
    std::vector<SpotLight> spotLights(0);

    static float time = 0.0f; 
    time += TimerManager::GetInstance()->GetDeltaTime();

    // Point Light 업데이트
    for (auto& light : pointLights)
    {
        light.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // 예시 값
        light.position = DirectX::XMFLOAT3(0.0f, 0.0f, 5.0f);    // 예시 값
        light.range = 3.0f;                                      // 예시 값
        light.time = time;
    }

    // Spot Light 업데이트
    for (auto& light : spotLights)
    {
        light.color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.8f, 1.0f); // 예시 값
        light.position = DirectX::XMFLOAT3(0.0f, 2.0f, 5.0f);   // 예시 값
        light.range = 5.0f;                                     // 예시 값
        light.direction = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);  // 예시 값
        light.angle = 10.0f;                                     // 예시 값
        light.time = time;
    }

    // Point Light Buffer에 데이터 전송
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = context->Map(d3d11_light_buffer_container_["PointLightBuffer"].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, pointLights.data(), sizeof(PointLight) * pointLights.size());
        context->Unmap(d3d11_light_buffer_container_["PointLightBuffer"].Get(), 0);
    }

    // Spot Light Buffer에 데이터 전송
    hr = context->Map(d3d11_light_buffer_container_["SpotLightBuffer"].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, spotLights.data(), sizeof(SpotLight) * spotLights.size());
        context->Unmap(d3d11_light_buffer_container_["SpotLightBuffer"].Get(), 0);
    }
}

ID3D11ShaderResourceView** LightsManager::GetLightShaderResourceView(std::string view_name)
{
    if (d3d11_shader_resource_view_container_.find(view_name) != d3d11_shader_resource_view_container_.end())
    {
        return d3d11_shader_resource_view_container_[view_name].GetAddressOf();
    }

    throw std::string("Light ShaderResource View Name Error");
} 
