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

	static DeviceManager* GetInstance()
	{
		if (instance_ == nullptr) 
		{
			instance_ = new DeviceManager();
		}

		return instance_;
	}

private:
	Microsoft::WRL::ComPtr<class ID3D11Device> device_;
	Microsoft::WRL::ComPtr<class ID3D11DeviceContext> immediate_context_;

private:
	class IDXGIAdapter1* GetAdapter();

public:
	void Initialize();
	class ID3D11Device* GetD3D11Device() const;
	class ID3D11DeviceContext* GetD3D11ImmediateContext() const;
}; 
