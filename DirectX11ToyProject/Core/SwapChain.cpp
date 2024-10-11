#include "SwapChain.h"
#include "../framework.h"

SwapChain* SwapChain::instance_ = nullptr;

void SwapChain::Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	swap_chain_desc.BufferDesc.Width = window_width;
	swap_chain_desc.BufferDesc.Height = window_height;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60; //TEST Change my monitor HZ
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;

	Microsoft::WRL::ComPtr<IDXGIDevice> previous_divice;
	Device::GetInstace()->GetD3D11Device()->QueryInterface(__uuidof(IDXGIDevice), (void**)previous_divice.GetAddressOf());

	Microsoft::WRL::ComPtr<IDXGIAdapter> previous_adapter;
	previous_divice->GetParent(__uuidof(IDXGIAdapter), (void**)previous_adapter.GetAddressOf());
	 
	Microsoft::WRL::ComPtr<IDXGIFactory> previous_factory;
	previous_adapter->GetParent(__uuidof(IDXGIFactory), (void**)previous_factory.GetAddressOf());

	HRESULT result = previous_factory->CreateSwapChain(Device::GetInstace()->GetD3D11Device(), &swap_chain_desc, d3d11_swap_chain_.GetAddressOf());
	 
	if (result != S_OK)
	{
		throw std::string("Can Not Create SwapChain");
	}
}

IDXGISwapChain* SwapChain::GetDXGISwapChain() const
{
	return d3d11_swap_chain_.Get();
}
