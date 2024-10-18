#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d11.h>

class Object
{
protected: 
	DirectX::XMFLOAT3A right_;
	DirectX::XMFLOAT3A up_;
	DirectX::XMFLOAT3A look_;
	DirectX::XMFLOAT3A position_;

protected:
	Microsoft::WRL::ComPtr<ID3D11CommandList> d3d11_command_list_;

public:
	Object();
	virtual ~Object() = default;

public:
	virtual void Initialize() = 0;
	void ExecuteCommandList();
	 
	DirectX::XMVECTOR GetRight() const;
	DirectX::XMVECTOR GetUp() const;
	DirectX::XMVECTOR GetLook() const;
	DirectX::XMVECTOR GetPosition() const;
};

