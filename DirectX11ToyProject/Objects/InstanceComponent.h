#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <memory>
#include <functional> // 람다 함수를 매개변수로 받기 위해 필요
#include "../Core/DeviceManager.h"

template <typename T>
class InstanceComponent
{
private: 
    std::unique_ptr<std::vector<T>> instances_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> instance_vertex_buffer_;

private:
    void CreateInstanceVertexBuffer();

public:
    // 람다 함수를 std::function 타입으로 매개변수로 받는다.
    void Initialize(std::function<std::unique_ptr<std::vector<T>>()> initializer);
    ID3D11Buffer* GetInstanceVertexBuffer() const;
    unsigned int GetInstanceCount() const;
    unsigned int GetStride() const;
    unsigned int GetOffset() const;
    std::vector<T>* GetInstances() const;
};

template<typename T>
inline void InstanceComponent<T>::CreateInstanceVertexBuffer()
{
    D3D11_BUFFER_DESC instance_buffer_desc;
    ZeroMemory(&instance_buffer_desc, sizeof(D3D11_BUFFER_DESC));
    instance_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    instance_buffer_desc.ByteWidth = sizeof(T) * instances_->size();
    instance_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    instance_buffer_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA instance_data;
    ZeroMemory(&instance_data, sizeof(D3D11_SUBRESOURCE_DATA));
    instance_data.pSysMem = instances_->data(); // 포인터로 벡터의 데이터를 전달
     
    DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&instance_buffer_desc, &instance_data, instance_vertex_buffer_.GetAddressOf());
}

template<typename T>
inline void InstanceComponent<T>::Initialize(std::function<std::unique_ptr<std::vector<T>>()> initializer)
{ 
    instances_ = initializer();  
      
    // 초기화 후 인스턴스 버퍼 생성
    CreateInstanceVertexBuffer();
}

template<typename T>
inline ID3D11Buffer* InstanceComponent<T>::GetInstanceVertexBuffer() const
{
    return instance_vertex_buffer_.Get();
}

template<typename T>
inline unsigned int InstanceComponent<T>::GetInstanceCount() const
{
    return instances_->size();
}

template<typename T>
inline unsigned int InstanceComponent<T>::GetStride() const
{
    return sizeof(T);
}

template<typename T>
inline unsigned int InstanceComponent<T>::GetOffset() const
{
    return 0;
}

template<typename T>
inline std::vector<T>* InstanceComponent<T>::GetInstances() const
{
    return instances_.get();
}
