#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include "Object.h"

class SingleObject : public Object
{
protected:
	DirectX::XMFLOAT4X4A world_matrix_;

	Microsoft::WRL::ComPtr<class ID3D11Buffer> world_matrix_constant_buffer_;

public:
	SingleObject();
	virtual ~SingleObject() = default;

protected:
	virtual void CreateBuffer() override;

public:
	virtual void UpdateBuffer() override;

public:
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

