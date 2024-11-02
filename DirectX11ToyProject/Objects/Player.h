#pragma once
#include "MovableObject.h"

class Player : public MovableObject
{
private:
	const DirectX::XMFLOAT3A kPlayerToCameraOffset = DirectX::XMFLOAT3A(2.5f, -2.0f, 1.0f); 

public:
	virtual void Initialize() override;
	
	void SetRotationAndPosition(DirectX::FXMMATRIX camera_world_matrix);
};

