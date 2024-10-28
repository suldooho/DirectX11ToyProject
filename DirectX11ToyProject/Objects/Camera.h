#pragma once
#include "Object.h"
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>

struct CameraMatrix
{ 
	DirectX::XMFLOAT4X4A view_matrix;
	DirectX::XMFLOAT4X4A projection_matrix;
	DirectX::XMFLOAT3A camera_position;
};

class Camera  
{
private:
	DirectX::XMFLOAT4X4A world_matrix_;

private:
	std::unique_ptr<class Player> player_;

	CameraMatrix camera_matrix_;
	Microsoft::WRL::ComPtr<class ID3D11Buffer> camera_matrix_constant_buffer_;
	 
private:
	float move_speed_; 

public:
	Camera();

private:
	void CreateConstantBuffer();
	void CreateProjectionMatrix(float near_plane_distance, float far_plane_distance, float aspect_ratio, float fov_angle);

public:
	void Initialize(float client_width, float client_height);

	class ID3D11Buffer** GetAddressOfCameraConstantBuffer(); 
	void UpdateViewMatrix();
	void UpdateConstantBuffer();

	void Move(DirectX::FXMVECTOR move_vector);

	void RotateYaw(float yaw);
	void RotatePitch(float pitch);

public:
	void SetPosition(DirectX::FXMVECTOR new_position);

	DirectX::XMVECTOR GetRight() const;
	DirectX::XMVECTOR GetUp() const;
	DirectX::XMVECTOR GetLook() const; 
	DirectX::XMVECTOR GetPosition() const;

	DirectX::XMMATRIX GetWorldMatrix();
};

