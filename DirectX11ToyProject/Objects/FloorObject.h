#pragma once
#include "Object.h"
#include "InstanceComponent.h"
#include "../Meshes/Vertices.h"

class FloorObject : public Object
{
public:
	std::unique_ptr<InstanceComponent<WorldInstanceData>> instance_component_;

private:
	void InitializeComponents();

public:
	virtual void Initialize() override;
	virtual void UpdateConstantBuffer() override;
};

