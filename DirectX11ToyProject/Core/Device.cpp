#include "Device.h"
#include "../framework.h"

IDXGIAdapter1* Device::GetAdapter()
{
	Microsoft::WRL::ComPtr<IDXGIFactory1> factory = nullptr;
	HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&factory));
	 
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
	DXGI_ADAPTER_DESC1 adapter_desc;
	while (factory->EnumAdapters1(NULL, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapter->GetDesc1(&adapter_desc); 
		if (wcsstr(adapter_desc.Description, L"NVIDIA") != nullptr)
		{
			return adapter.Get();
		}
	}

	throw std::string("Not Found NVIDIA Adapter");  
}

void Device::Initialize()
{
	IDXGIAdapter1* adapter = GetAdapter();
}
