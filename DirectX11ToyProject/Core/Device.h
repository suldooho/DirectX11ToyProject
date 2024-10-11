#pragma once
#include <wrl.h>

class Device
{
private:
	static Device* instance_;

	Device() = default;
	~Device() = default;

public:
	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;

	static Device* GetInstace()
	{
		if (instance_ == nullptr) 
		{
			instance_ = new Device();
		}

		return instance_;
	}

private:
	Microsoft::WRL::ComPtr<class ID3D11Device> d3d11_device_;
	Microsoft::WRL::ComPtr<class ID3D11DeviceContext> d3d11_immediate_context_;

private:
	class IDXGIAdapter1* GetAdapter();

public:
	void Initialize();
	class ID3D11Device* GetD3D11Device() const; 
}; 
