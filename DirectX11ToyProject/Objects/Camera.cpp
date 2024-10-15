#include "Camera.h"
#include "../framework.h"

void Camera::CreateViewMatrix(DirectX::FXMVECTOR eye_position, DirectX::FXMVECTOR focus_position, DirectX::FXMVECTOR up_direction)
{
	DirectX::XMMATRIX view_matrix = DirectX::XMMatrixLookAtLH(eye_position, focus_position, up_direction);
	DirectX::XMStoreFloat4x4A(&view_matrix_, view_matrix);
}

void Camera::CreateProjectionMatrix(float fov_angle, float aspect_ratio, float near_plane_distance, float far_plane_distance)
{
	DirectX::XMMATRIX projection_matrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov_angle), aspect_ratio, near_plane_distance, far_plane_distance);
	DirectX::XMStoreFloat4x4A(&projection_matrix_, projection_matrix);
}
