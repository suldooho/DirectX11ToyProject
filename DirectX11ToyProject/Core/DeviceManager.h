#pragma once
#include <wrl.h>

class DeviceManager
{
private:
	static DeviceManager* instance_;

	DeviceManager() = default;
	~DeviceManager() = default;

public:
	DeviceManager(const DeviceManager&) = delete;
	DeviceManager& operator=(const DeviceManager&) = delete;

	static DeviceManager* GetInstace()
	{
		if (instance_ == nullptr) 
		{
			instance_ = new DeviceManager();
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
