#pragma once
#include "InstanceObject.h" 
#include "../Meshes/Vertices.h"

class FloorObject : public InstanceObject<PositionInstanceData>
{ 
public:
	virtual void Initialize() override;
};

