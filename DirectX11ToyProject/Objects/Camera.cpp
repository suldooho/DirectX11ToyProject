#include "Camera.h"
#include "../framework.h"
#include "Player.h"

Camera::Camera()
{ 
	DirectX::XMStoreFloat3A(&right_, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3A(&up_, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3A(&look_, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	DirectX::XMStoreFloat3A(&position_, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
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
	CreateConstantBuffer();
	CreateProjectionMatrix(90.0f, client_width / client_height, 1.0f, 500.0f);
}

ID3D11Buffer** Camera::GetAddressOfCameraConstantBuffer()
{
	return d3d11_camera_matrix_constant_buffer_.GetAddressOf();
}

void Camera::SetPlayer(Player* player)
{
	player_.reset(player);
	DirectX::XMStoreFloat3A(&init_player_position_offset_, DirectX::XMVectorMultiply(player_->GetPosition(), DirectX::XMVectorSet(-1.0f, -1.0f, -1.0f, 1.0f)));
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR eye_position = DirectX::XMVectorAdd(player_->GetPosition(), DirectX::XMLoadFloat3A(&init_player_position_offset_));
	DirectX::XMVECTOR focus_position = DirectX::XMVectorAdd(eye_position, DirectX::XMLoadFloat3A(&look_));
	DirectX::XMMATRIX view_matrix = DirectX::XMMatrixLookAtLH(eye_position, focus_position, player_->GetUp());
	DirectX::XMStoreFloat4x4A(&camera_matrix_.view_matrix, view_matrix);
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
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->Unmap(d3d11_camera_matrix_constant_buffer_.Get(), 0);
}
