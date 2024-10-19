#pragma once
#include "Object.h"
#include <wrl.h>

class MovableObject : public Object
{
protected:
	Microsoft::WRL::ComPtr<class ID3D11Buffer> d3d11_world_matrix_constant_buffer_;

	float move_speed_;
	 
public:
	virtual void Initialize() override;

	void SetMoveSpeed(float move_speed);
	void Move(DirectX::FXMVECTOR move_vector);

	void UpdateConstantBuffer();
};

