#include "SingleObject.h"
#include "../framework.h"

SingleObject::SingleObject() : Object()
{
	DirectX::XMStoreFloat4x4A(&world_matrix_, DirectX::XMMatrixIdentity());
}

void SingleObject::CreateBuffer()
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4A);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	DeviceManager::GetInstance()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &world_matrix_constant_buffer_);
}
 

void SingleObject::UpdateBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->Map(world_matrix_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	DirectX::XMFLOAT4X4A* world_matrix = reinterpret_cast<DirectX::XMFLOAT4X4A*>(mapped_subresource.pData);
	DirectX::XMMATRIX xm_world_matrix = DirectX::XMLoadFloat4x4A(&world_matrix_);
	xm_world_matrix = DirectX::XMMatrixTranspose(xm_world_matrix);
	DirectX::XMStoreFloat4x4A(world_matrix, xm_world_matrix);
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->Unmap(world_matrix_constant_buffer_.Get(), 0);
}

void SingleObject::SetRight(float x, float y, float z)
{
	world_matrix_._11 = x;
	world_matrix_._12 = y;
	world_matrix_._13 = z;
}

void SingleObject::SetUp(float x, float y, float z)
{
	world_matrix_._21 = x;
	world_matrix_._22 = y;
	world_matrix_._23 = z;
}

void SingleObject::SetLook(float x, float y, float z)
{
	world_matrix_._31 = x;
	world_matrix_._32 = y;
	world_matrix_._33 = z;
}

void SingleObject::SetPosition(float x, float y, float z)
{
	world_matrix_._41 = x;
	world_matrix_._42 = y;
	world_matrix_._43 = z;
}

void SingleObject::SetPosition(DirectX::FXMVECTOR new_position)
{
	world_matrix_._41 = DirectX::XMVectorGetX(new_position);
	world_matrix_._42 = DirectX::XMVectorGetY(new_position);
	world_matrix_._43 = DirectX::XMVectorGetZ(new_position);
}

DirectX::XMVECTOR SingleObject::GetRight() const
{
	return DirectX::XMVectorSet(world_matrix_._11, world_matrix_._12, world_matrix_._13, 0.0f);
}

DirectX::XMVECTOR SingleObject::GetUp() const
{
	return DirectX::XMVectorSet(world_matrix_._21, world_matrix_._22, world_matrix_._23, 0.0f);
}

DirectX::XMVECTOR SingleObject::GetLook() const
{
	return DirectX::XMVectorSet(world_matrix_._31, world_matrix_._32, world_matrix_._33, 0.0f);
}

DirectX::XMVECTOR SingleObject::GetPosition() const
{
	return DirectX::XMVectorSet(world_matrix_._41, world_matrix_._42, world_matrix_._43, 1.0f);
} 
