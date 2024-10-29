#pragma once
#include "Object.h"
#include <wrl.h>

class MovableObject : public Object
{
protected: 
	float move_speed_;
	 
public:  
	void SetMoveSpeed(float move_speed);
	void Move(DirectX::FXMVECTOR move_vector);
};

