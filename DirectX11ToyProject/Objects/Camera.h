#pragma once
#include "Object.h"
#include <DirectXMath.h>

class Camera : public Object
{
private:
	DirectX::XMFLOAT4X4A view_matrix_;
	DirectX::XMFLOAT4X4A projection_matrix_;

public:
	void CreateViewMatrix(DirectX::FXMVECTOR eye_position, DirectX::FXMVECTOR focus_position, DirectX::FXMVECTOR up_direction);
	void CreateProjectionMatrix(float near_plane_distance, float far_plane_distance, float aspect_ratio, float fov_angle);
};

