#pragma once
#include <wrl.h>

class Device
{
private:
	Microsoft::WRL::ComPtr<class ID3D11Device> d3d11_device_;

private:
	class IDXGIAdapter1* GetAdapter();

public:
	void Initialize();


};

