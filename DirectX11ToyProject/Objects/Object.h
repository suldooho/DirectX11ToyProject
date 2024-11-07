#pragma once 
#include <wrl.h> 
#include <d3d11.h>

class Object
{ 
protected: 
	Microsoft::WRL::ComPtr<ID3D11CommandList> command_list_;

protected:
	virtual void CreateBuffer() = 0;

public:
	virtual void Initialize() = 0;
	virtual void UpdateBuffer() = 0;
	virtual void AnimateObject();
	void ExecuteCommandList();
};

