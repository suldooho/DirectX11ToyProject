#pragma once
#include "InstanceObject.h"
#include "../Meshes/Vertices.h"

class EnemyObject : public InstanceObject<EnemyInstanceData>
{
public:
	virtual void Initialize() override;
	virtual void AnimateObject() override;
};

