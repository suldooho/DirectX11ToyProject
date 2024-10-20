#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d11.h>

class Object
{
protected:
	DirectX::XMFLOAT4X4A world_matrix_;

protected:
	Microsoft::WRL::ComPtr<ID3D11CommandList> d3d11_command_list_;

public:
	Object();
	virtual ~Object() = default;

public:
	virtual void Initialize() = 0;
	void ExecuteCommandList();

	void SetRight(float x, float y, float z);
	void SetUp(float x, float y, float z);
	void SetLook(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::FXMVECTOR new_position);
	 
	DirectX::XMVECTOR GetRight() const;
	DirectX::XMVECTOR GetUp() const;
	DirectX::XMVECTOR GetLook() const;
	DirectX::XMVECTOR GetPosition() const;
};

