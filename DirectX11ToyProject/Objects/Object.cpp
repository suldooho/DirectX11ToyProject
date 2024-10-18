#include "Object.h"
#include "../framework.h"

Object::Object()
{
	DirectX::XMStoreFloat3A(&right_, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3A(&up_, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3A(&look_, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	DirectX::XMStoreFloat3A(&position_, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)); 
}

void Object::ExecuteCommandList()
{
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->ExecuteCommandList(d3d11_command_list_.Get(), true);
}

DirectX::XMVECTOR Object::GetRight() const
{
	return DirectX::XMLoadFloat3A(&right_);
}

DirectX::XMVECTOR Object::GetUp() const
{
	return DirectX::XMLoadFloat3A(&up_);
}

DirectX::XMVECTOR Object::GetLook() const
{
	return DirectX::XMLoadFloat3A(&look_);
}

DirectX::XMVECTOR Object::GetPosition() const
{
	return DirectX::XMLoadFloat3A(&position_);
}
