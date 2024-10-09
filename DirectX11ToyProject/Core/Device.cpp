#include "Device.h"
#include "../framework.h"

IDXGIAdapter1* Device::GetAdapter()
{
	Microsoft::WRL::ComPtr<IDXGIFactory1> factory = nullptr;
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&factory));
	 
	int next_adapter = 0;
	IDXGIAdapter1* adapter = nullptr;
	DXGI_ADAPTER_DESC1 adapter_desc;
	while (factory->EnumAdapters1(next_adapter++, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapter->GetDesc1(&adapter_desc); 
		if (wcsstr(adapter_desc.Description, L"NVIDIA") != nullptr)
		{
			return adapter;
		}
	}

	throw std::string("Can Not Found NVIDIA Adapter");  
}

void Device::Initialize()
{
	IDXGIAdapter1* adapter = GetAdapter();

	D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_UNKNOWN;

	unsigned int create_device_flags = 0;
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };
	unsigned int num_feature_level = 1;

	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	 
	HRESULT result = D3D11CreateDevice(adapter, driver_type, NULL, create_device_flags, feature_levels, num_feature_level, D3D11_SDK_VERSION, &d3d11_device_, &feature_level, &d3d11_immediate_context_);
	
	if (result != S_OK)
	{
		throw std::string("Can Not Create Device");
	} 
}
