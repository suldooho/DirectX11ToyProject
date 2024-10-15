#pragma once
#include <wrl.h>
#include <DirectXMath.h>

class Object
{
protected:
	DirectX::XMFLOAT4X4A world_matrix_;

protected: 
	Microsoft::WRL::ComPtr<class ID3D11CommandList> d3d11_command_list_;

public:
	virtual void Initialize() = 0;
};

