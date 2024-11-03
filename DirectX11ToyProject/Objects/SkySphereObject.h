#pragma once
#include "SingleObject.h"

class SkySphereObject : public SingleObject
{
public:
	SkySphereObject() = default;
	virtual ~SkySphereObject() = default;

public:
	virtual void Initialize() override;
};

