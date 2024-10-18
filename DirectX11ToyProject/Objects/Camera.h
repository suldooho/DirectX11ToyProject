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
	std::unique_ptr<class Player> player_;

	CameraMatrix camera_matrix_;
	Microsoft::WRL::ComPtr<class ID3D11Buffer> d3d11_camera_matrix_constant_buffer_;
	 
private:
	const DirectX::XMFLOAT3A kUpOffset = DirectX::XMFLOAT3A(0.0f, 2.0f, 0.0f);
	const DirectX::XMFLOAT3A kBackOffset = DirectX::XMFLOAT3A(0.0f, 0.0f, -1.0f);
	 
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

