#pragma once
#include "SingleObject.h"

class Player : public SingleObject
{
private:
	float move_speed_;
	const DirectX::XMFLOAT3A kPlayerToCameraOffset = DirectX::XMFLOAT3A(2.5f, -2.0f, 1.0f); 

public:
	virtual void Initialize() override;
	
	void SetRotationAndPosition(DirectX::FXMMATRIX camera_world_matrix);
	 
	void SetMoveSpeed(float move_speed);
	void Move(DirectX::FXMVECTOR move_vector);
};

