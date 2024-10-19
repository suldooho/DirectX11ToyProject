#pragma once
#include "Object.h"
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>

struct CameraMatrix
{ 
	DirectX::XMFLOAT4X4A view_matrix;
	DirectX::XMFLOAT4X4A projection_matrix;
};

class Camera  
{
private:
	DirectX::XMFLOAT3A right_;
	DirectX::XMFLOAT3A up_;
	DirectX::XMFLOAT3A look_;
	DirectX::XMFLOAT3A position_; 

private:
	std::unique_ptr<class Player> player_;

	CameraMatrix camera_matrix_;
	Microsoft::WRL::ComPtr<class ID3D11Buffer> d3d11_camera_matrix_constant_buffer_;
	 
private:
	const DirectX::XMFLOAT3A kPositionOffset = DirectX::XMFLOAT3A(0.0f, 0.0f, -3.0f); 
	DirectX::XMFLOAT3A init_player_position_offset_;

public:
	Camera();

private:
	void CreateConstantBuffer();
	void CreateProjectionMatrix(float near_plane_distance, float far_plane_distance, float aspect_ratio, float fov_angle);

public:
	void Initialize(float client_width, float client_height);

	class ID3D11Buffer** GetAddressOfCameraConstantBuffer();
	void SetPlayer(class Player* player);
	void UpdateViewMatrix();
	void UpdateConstantBuffer();
};

