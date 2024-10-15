#include "OutputMerger.h"
#include "../framework.h"

void OutputMerger::CreateRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	SwapChainManager::GetInstace()->GetDXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
	HRESULT result = DeviceManager::GetInstace()->GetD3D11Device()->CreateRenderTargetView(back_buffer.Get(), 0, d3d11_render_target_view_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Render Target View");
	}
}

void OutputMerger::CreateDepthStencilView(unsigned int client_width, unsigned int client_height)
{
	D3D11_TEXTURE2D_DESC depth_stencil_desc;
	depth_stencil_desc.Width = client_width;
	depth_stencil_desc.Height = client_height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	DeviceManager::GetInstace()->GetD3D11Device()->CreateTexture2D(&depth_stencil_desc, 0, d3d11_depth_stencil_buffer_.GetAddressOf());

	HRESULT result = DeviceManager::GetInstace()->GetD3D11Device()->CreateDepthStencilView(d3d11_depth_stencil_buffer_.Get(), 0, d3d11_depth_stencil_view_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil View");
	}
}

void OutputMerger::CreateViewPort(unsigned int client_width, unsigned int client_height)
{
	d3d11_viewport_ = std::make_unique<D3D11_VIEWPORT>();
	d3d11_viewport_.get()->TopLeftX = 0.0f;
	d3d11_viewport_.get()->TopLeftY = 0.0f;
	d3d11_viewport_.get()->Width = static_cast<float>(client_width);
	d3d11_viewport_.get()->Height = static_cast<float>(client_height);
	d3d11_viewport_.get()->MinDepth = 0.0f;
	d3d11_viewport_.get()->MaxDepth = 1.0f;
}

void OutputMerger::Initialize(unsigned int client_width, unsigned int client_height)
{
	CreateRenderTargetView();
	CreateDepthStencilView(client_width, client_height);
	CreateViewPort(client_width, client_height);
}

ID3D11RenderTargetView* OutputMerger::GetRenderTargetView() const
{
	return d3d11_render_target_view_.Get();
}

ID3D11DepthStencilView* OutputMerger::GetDepthStencilView() const
{
	return d3d11_depth_stencil_view_.Get();
}

D3D11_VIEWPORT* OutputMerger::GetViewport() const
{
	return d3d11_viewport_.get();
}
