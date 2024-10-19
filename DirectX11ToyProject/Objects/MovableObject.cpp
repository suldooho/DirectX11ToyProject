#include "MovableObject.h"
#include "../framework.h" 

void MovableObject::Initialize()
{
	move_speed_ = 10.0f;

	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4A);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	DeviceManager::GetInstace()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &d3d11_world_matrix_constant_buffer_);
}

void MovableObject::SetMoveSpeed(float move_speed)
{
	move_speed_ = move_speed;
}

void MovableObject::Move(DirectX::FXMVECTOR move_vector)
{ 
	DirectX::XMVECTOR new_position = DirectX::XMVectorAdd(GetPosition(), DirectX::XMVectorScale(move_vector, move_speed_));
	DirectX::XMStoreFloat3A(&position_, new_position);
}

void MovableObject::UpdateConstantBuffer()
{  
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->Map(d3d11_world_matrix_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	DirectX::XMFLOAT4X4A* world_matrix = reinterpret_cast<DirectX::XMFLOAT4X4A*>(mapped_subresource.pData);
	DirectX::XMFLOAT4A position = DirectX::XMFLOAT4A(position_.x, position_.y, position_.z, 1.0f);
	DirectX::XMMATRIX xm_world_matrix = DirectX::XMMATRIX(DirectX::XMLoadFloat3A(&right_), DirectX::XMLoadFloat3A(&up_), DirectX::XMLoadFloat3A(&look_), DirectX::XMLoadFloat4A(&position));
	xm_world_matrix = DirectX::XMMatrixTranspose(xm_world_matrix);
	DirectX::XMStoreFloat4x4A(world_matrix, xm_world_matrix);
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->Unmap(d3d11_world_matrix_constant_buffer_.Get(), 0); 
}


