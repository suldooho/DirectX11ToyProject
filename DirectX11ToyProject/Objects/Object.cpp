#include "Object.h"
#include "../framework.h"

Object::Object()
{
	DirectX::XMStoreFloat4x4A(&world_matrix_, DirectX::XMMatrixIdentity());
}

void Object::ExecuteCommandList()
{
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->ExecuteCommandList(d3d11_command_list_.Get(), true);
}

void Object::SetRight(float x, float y, float z)
{
	world_matrix_._11 = x;
	world_matrix_._12 = y;
	world_matrix_._13 = z; 
}

void Object::SetUp(float x, float y, float z)
{
	world_matrix_._21 = x;
	world_matrix_._22 = y;
	world_matrix_._23 = z;
}

void Object::SetLook(float x, float y, float z)
{
	world_matrix_._31 = x;
	world_matrix_._32 = y;
	world_matrix_._33 = z;
}

void Object::SetPosition(float x, float y, float z)
{
	world_matrix_._41 = x;
	world_matrix_._42 = y;
	world_matrix_._43 = z;
}

void Object::SetPosition(DirectX::FXMVECTOR new_position)
{
	world_matrix_._41 = DirectX::XMVectorGetX(new_position);
	world_matrix_._42 = DirectX::XMVectorGetY(new_position);
	world_matrix_._43 = DirectX::XMVectorGetZ(new_position);
}

DirectX::XMVECTOR Object::GetRight() const
{
	return DirectX::XMVectorSet(world_matrix_._11, world_matrix_._12, world_matrix_._13, 0.0f);
}

DirectX::XMVECTOR Object::GetUp() const
{
	return DirectX::XMVectorSet(world_matrix_._21, world_matrix_._22, world_matrix_._23, 0.0f);
}

DirectX::XMVECTOR Object::GetLook() const
{
	return DirectX::XMVectorSet(world_matrix_._31, world_matrix_._32, world_matrix_._33, 0.0f);
}

DirectX::XMVECTOR Object::GetPosition() const
{
	return DirectX::XMVectorSet(world_matrix_._41, world_matrix_._42, world_matrix_._43, 1.0f);
}
