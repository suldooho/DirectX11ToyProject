#pragma once
#include <vector>
#include <wrl.h> 
#include <d3d11.h>
#include <memory>
#include <functional>
#include "Object.h"
#include "../Core/DeviceManager.h"

template <typename T>
class InstanceObject : public Object
{
protected:
    std::unique_ptr<std::vector<T>> instances_;

    Microsoft::WRL::ComPtr<ID3D11Buffer> instance_vertex_buffer_; 
     
public:
    InstanceObject();
    virtual ~InstanceObject() = default;

protected:
    virtual void CreateBuffer() override;

public:
    virtual void UpdateBuffer() override;
     
public:
    void SetInitializeValue(std::function<std::unique_ptr<std::vector<T>>()> initializer);  // 람다 함수를 std::function 타입으로 매개변수로 받는다.
};

template<typename T>
inline InstanceObject<T>::InstanceObject()
{
}

template<typename T>
inline void InstanceObject<T>::CreateBuffer()
{
    D3D11_BUFFER_DESC instance_buffer_desc;
    ZeroMemory(&instance_buffer_desc, sizeof(D3D11_BUFFER_DESC));
    instance_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    instance_buffer_desc.ByteWidth = sizeof(T) * instances_->size();
    instance_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    instance_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA instance_data;
    ZeroMemory(&instance_data, sizeof(D3D11_SUBRESOURCE_DATA));
    instance_data.pSysMem = instances_->data();  

    DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&instance_buffer_desc, &instance_data, instance_vertex_buffer_.GetAddressOf());
}

template<typename T>
inline void InstanceObject<T>::UpdateBuffer()
{
    D3D11_MAPPED_SUBRESOURCE mapped_subresource;
    DeviceManager::GetInstance()->GetD3D11ImmediateContext()->Map(instance_vertex_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    memcpy(mapped_subresource.pData, instances_->data(), sizeof(T) * instances_->size()); // 데이터를 버퍼에 복사
    DeviceManager::GetInstance()->GetD3D11ImmediateContext()->Unmap(instance_vertex_buffer_.Get(), 0);
}

template<typename T>
inline void InstanceObject<T>::SetInitializeValue(std::function<std::unique_ptr<std::vector<T>>()> initializer)
{
    instances_ = initializer();
}

