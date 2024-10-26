#include "Camera.h"
#include "../framework.h"
#include "Player.h"

Camera::Camera()
{ 
	DirectX::XMStoreFloat4x4A(&world_matrix_, DirectX::XMMatrixIdentity());
}

void Camera::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = sizeof(CameraMatrix);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	DeviceManager::GetInstace()->GetD3D11Device()->CreateBuffer(&buffer_desc, nullptr, &d3d11_camera_matrix_constant_buffer_);
}

void Camera::CreateProjectionMatrix(float fov_angle, float aspect_ratio, float near_plane_distance, float far_plane_distance)
{
	DirectX::XMMATRIX projection_matrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov_angle), aspect_ratio, near_plane_distance, far_plane_distance);
	DirectX::XMStoreFloat4x4A(&camera_matrix_.projection_matrix, projection_matrix);
}

void Camera::Initialize(float client_width, float client_height)
{ 
	move_speed_ = 20.0f;
	CreateConstantBuffer();
	CreateProjectionMatrix(90.0f, client_width / client_height, 1.0f, 500.0f);
}

ID3D11Buffer** Camera::GetAddressOfCameraConstantBuffer()
{
	return d3d11_camera_matrix_constant_buffer_.GetAddressOf();
}
 
void Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR right = DirectX::XMVectorSet(world_matrix_._11, world_matrix_._12, world_matrix_._13, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(world_matrix_._21, world_matrix_._22, world_matrix_._23, 0.0f);
	DirectX::XMVECTOR look = DirectX::XMVectorSet(world_matrix_._31, world_matrix_._32, world_matrix_._33, 0.0f);
	DirectX::XMVECTOR position = DirectX::XMVectorSet(world_matrix_._41, world_matrix_._42, world_matrix_._43, 0.0f);
	
	float view_position_x = DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, right)) * -1.0f;
	float view_position_y = DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, up)) * -1.0f;
	float view_position_z = DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, look)) * -1.0f;

	DirectX::XMMATRIX view_matrix = DirectX::XMMATRIX(right, up, look, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	view_matrix = DirectX::XMMatrixTranspose(view_matrix);
	view_matrix.r[3] = DirectX::XMVectorSet(view_position_x, view_position_y, view_position_z, 1.0f);

	DirectX::XMStoreFloat4x4A(&(camera_matrix_.view_matrix), view_matrix);
} 

void Camera::UpdateConstantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->Map(d3d11_camera_matrix_constant_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	CameraMatrix* camera_matrix = reinterpret_cast<CameraMatrix*>(mapped_subresource.pData);
	DirectX::XMMATRIX xm_view_matrix = DirectX::XMLoadFloat4x4A(&camera_matrix_.view_matrix);         
	DirectX::XMMATRIX xm_projection_matrix = DirectX::XMLoadFloat4x4A(&camera_matrix_.projection_matrix);
	xm_view_matrix = DirectX::XMMatrixTranspose(xm_view_matrix);
	xm_projection_matrix = DirectX::XMMatrixTranspose(xm_projection_matrix);
	DirectX::XMStoreFloat4x4A(&camera_matrix->view_matrix, xm_view_matrix);
	DirectX::XMStoreFloat4x4A(&camera_matrix->projection_matrix, xm_projection_matrix);
	camera_matrix->camera_position.x = world_matrix_._41;
	camera_matrix->camera_position.y = world_matrix_._42;
	camera_matrix->camera_position.z = world_matrix_._43;
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->Unmap(d3d11_camera_matrix_constant_buffer_.Get(), 0);
}

void Camera::Move(DirectX::FXMVECTOR move_vector)
{
	DirectX::XMVECTOR new_position = DirectX::XMVectorAdd(GetPosition(), DirectX::XMVectorScale(move_vector, move_speed_));
	SetPosition(new_position);
} 

void Camera::RotateYaw(float yaw)
{  
	DirectX::XMVECTOR up_vector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationAxis(up_vector, yaw);

	DirectX::XMFLOAT3 save_position = DirectX::XMFLOAT3(world_matrix_._41, world_matrix_._42, world_matrix_._43);
	world_matrix_._41 = 0.0f;
	world_matrix_._42 = 0.0f;
	world_matrix_._43 = 0.0f;

	DirectX::XMMATRIX rotated_matrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4A(&world_matrix_), rotation_matrix);

	DirectX::XMStoreFloat4x4A(&world_matrix_, rotated_matrix);
	world_matrix_._41 = save_position.x;
	world_matrix_._42 = save_position.y;
	world_matrix_._43 = save_position.z;
}

void Camera::RotatePitch(float pitch)
{
	DirectX::XMVECTOR right_vector = DirectX::XMVectorSet(world_matrix_._11, world_matrix_._12, world_matrix_._13, 0.0f);
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationAxis(right_vector, pitch);

	DirectX::XMFLOAT3 save_position = DirectX::XMFLOAT3(world_matrix_._41, world_matrix_._42, world_matrix_._43);
	world_matrix_._41 = 0.0f;
	world_matrix_._42 = 0.0f;
	world_matrix_._43 = 0.0f;

	DirectX::XMMATRIX rotated_matrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4A(&world_matrix_), rotation_matrix);

	DirectX::XMStoreFloat4x4A(&world_matrix_, rotated_matrix);
	world_matrix_._41 = save_position.x;
	world_matrix_._42 = save_position.y;
	world_matrix_._43 = save_position.z;
}

void Camera::SetPosition(DirectX::FXMVECTOR new_position)
{
	world_matrix_._41 = DirectX::XMVectorGetX(new_position);
	world_matrix_._42 = DirectX::XMVectorGetY(new_position);
	world_matrix_._43 = DirectX::XMVectorGetZ(new_position);
}

DirectX::XMVECTOR Camera::GetRight() const
{
	return DirectX::XMVectorSet(world_matrix_._11, world_matrix_._12, world_matrix_._13, 0.0f);
}

DirectX::XMVECTOR Camera::GetUp() const
{
	return DirectX::XMVectorSet(world_matrix_._21, world_matrix_._22, world_matrix_._23, 0.0f);
}

DirectX::XMVECTOR Camera::GetLook() const
{
	return DirectX::XMVectorSet(world_matrix_._31, world_matrix_._32, world_matrix_._33, 0.0f);
}

DirectX::XMVECTOR Camera::GetPosition() const
{
	return DirectX::XMVectorSet(world_matrix_._41, world_matrix_._42, world_matrix_._43, 1.0f);
}

DirectX::XMMATRIX Camera::GetWorldMatrix()
{
	return DirectX::XMLoadFloat4x4A(&world_matrix_);
}
