#pragma once
#include "InstanceObject.h" 
#include "../Meshes/Vertices.h"

class FloorObject : public InstanceObject<PositionInstanceData>
{
protected:
	Microsoft::WRL::ComPtr<ID3D11CommandList> test_command_list_;
	 
public:
	virtual void Initialize() override;
	void TestInitialize();

	void ExecuteTestCommandList();
};

