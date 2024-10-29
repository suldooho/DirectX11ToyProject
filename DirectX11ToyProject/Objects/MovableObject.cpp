#include "MovableObject.h"
#include "../framework.h"  

void MovableObject::SetMoveSpeed(float move_speed)
{
	move_speed_ = move_speed;
}

void MovableObject::Move(DirectX::FXMVECTOR move_vector)
{ 
	DirectX::XMVECTOR new_position = DirectX::XMVectorAdd(GetPosition(), DirectX::XMVectorScale(move_vector, move_speed_));
	SetPosition(new_position); 
} 


