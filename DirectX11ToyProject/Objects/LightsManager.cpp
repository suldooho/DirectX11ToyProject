#include "LightsManager.h"
#include "../framework.h"
#include "../Objects/EnemyObject.h"

LightsManager* LightsManager::instance_ = nullptr;

void LightsManager::Initialize()
{ 
    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.ByteWidth = sizeof(LightCount);
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    ID3D11Buffer* light_count_buffer = nullptr; 

    LightCount light_count_data;
    light_count_data.point_light_count = 0; 
    light_count_data.spot_light_count = 0;   
    D3D11_SUBRESOURCE_DATA init_data;
    ZeroMemory(&init_data, sizeof(D3D11_SUBRESOURCE_DATA));
    init_data.pSysMem = &light_count_data;

    HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, &init_data, &light_count_buffer);
    if (result != S_OK)
    {
        throw std::string("Failed To Create Light Count Buffer");
    }
    light_buffer_container_["LightCountBuffer"] = light_count_buffer;
     

    ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.ByteWidth = sizeof(PointLight) * kMaxPointLightNum_;
    buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    buffer_desc.StructureByteStride = sizeof(PointLight);
    ID3D11Buffer* point_light_buffer = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &point_light_buffer);
    if (result != S_OK)
    {
        throw std::string("Failed To Create Point Light Buffer");
    }
    light_buffer_container_["PointLightBuffer"] = point_light_buffer;
     
    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
    ZeroMemory(&shader_resource_view_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shader_resource_view_desc.Format = DXGI_FORMAT_UNKNOWN;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    shader_resource_view_desc.Buffer.FirstElement = 0;
    shader_resource_view_desc.Buffer.NumElements = kMaxPointLightNum_;
    ID3D11ShaderResourceView* point_light_shader_resource_view = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateShaderResourceView(light_buffer_container_["PointLightBuffer"].Get(), &shader_resource_view_desc, &point_light_shader_resource_view);
    if (result != S_OK)
    {
        throw std::string("Failed To Create Point Light Shader Resource View");
    }
    shader_resource_view_container_["PointLightView"] = point_light_shader_resource_view;

     
    buffer_desc.ByteWidth = sizeof(SpotLight) * kMaxSpotLightNum_;
    buffer_desc.StructureByteStride = sizeof(SpotLight); 
    ID3D11Buffer* spot_light_buffer = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &spot_light_buffer); 
    if (result != S_OK)
    {
        throw std::string("Failed To Create Spot Light Buffer");
    }
    light_buffer_container_["SpotLightBuffer"] = spot_light_buffer;

    shader_resource_view_desc.Buffer.NumElements = kMaxSpotLightNum_;
    ID3D11ShaderResourceView* spot_light_shader_resource_view = nullptr;
    result = DeviceManager::GetInstance()->GetD3D11Device()->CreateShaderResourceView(light_buffer_container_["SpotLightBuffer"].Get(), &shader_resource_view_desc, &spot_light_shader_resource_view);
    if (result != S_OK)
    {
        throw std::string("Failed to create Spot Light Shader Resource View");
    }
    shader_resource_view_container_["SpotLightView"] = spot_light_shader_resource_view;  
}

void LightsManager::UpdateLightBuffers()
{
    ID3D11DeviceContext* context = DeviceManager::GetInstance()->GetD3D11ImmediateContext();

    EnemyObject* enemies = ObjectsManager::GetInstance()->GetEnemies();
     
    LightCount light_count;
    if (ObjectsManager::GetInstance()->GetIsStart())
    {
        light_count.point_light_count = enemies->GetEnemiesCount();
        light_count.spot_light_count = 0;
    }
    else
    {
        light_count.point_light_count = 0;
        light_count.spot_light_count = 0;
    }

    // 조명 데이터 생성 (예시로 더미 데이터 설정)
    std::vector<PointLight> pointLights(light_count.point_light_count);
    std::vector<SpotLight> spotLights(light_count.spot_light_count);

    static float time = 0.0f; 
    time += TimerManager::GetInstance()->GetDeltaTime();

    // Point Light 업데이트
    for (unsigned int i = 0; i < pointLights.size(); ++i)
    {
        const EnemyInstanceData* enemy_data = enemies->GetEnemiesDataOfIndex(i);
        pointLights[i].color = enemy_data->color; 
        pointLights[i].position.x = enemy_data->world3.x;
        pointLights[i].position.y = enemy_data->world3.y;
        pointLights[i].position.z = enemy_data->world3.z;
        pointLights[i].range = 3.0f;
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
    HRESULT hr = context->Map(light_buffer_container_["LightCountBuffer"].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, &light_count, sizeof(LightCount));
        context->Unmap(light_buffer_container_["LightCountBuffer"].Get(), 0);
    }

    hr = context->Map(light_buffer_container_["PointLightBuffer"].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, pointLights.data(), sizeof(PointLight) * pointLights.size());
        context->Unmap(light_buffer_container_["PointLightBuffer"].Get(), 0);
    }

    // Spot Light Buffer에 데이터 전송
    hr = context->Map(light_buffer_container_["SpotLightBuffer"].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, spotLights.data(), sizeof(SpotLight) * spotLights.size());
        context->Unmap(light_buffer_container_["SpotLightBuffer"].Get(), 0);
    }
}

ID3D11ShaderResourceView** LightsManager::GetLightShaderResourceView(std::string view_name)
{
    if (shader_resource_view_container_.find(view_name) != shader_resource_view_container_.end())
    {
        return shader_resource_view_container_[view_name].GetAddressOf();
    }

    throw std::string("Light Shader Resource View Name Error");
}
ID3D11Buffer** LightsManager::GetLightBuffer(std::string buffer_name)
{
    if (light_buffer_container_.find(buffer_name) != light_buffer_container_.end())
    {
        return light_buffer_container_[buffer_name].GetAddressOf();
    }

    throw std::string("Light Shader Resource View Name Error");
}

