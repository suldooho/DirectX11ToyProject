#include "OutputMerger.h"
#include "../framework.h"

ID3D11Texture2D* OutputMerger::CreateGBufferTexture(DXGI_FORMAT format, unsigned int client_width, unsigned int client_height)
{
	D3D11_TEXTURE2D_DESC texture_desc;
	ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
	texture_desc.Width = client_width;
	texture_desc.Height = client_height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = format;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* texture = nullptr;
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateTexture2D(&texture_desc, nullptr, &texture);
	
	if (result != S_OK) 
	{
		throw std::string("Can Not Create GBuffer Texture");
	}

	return texture;
} 

void OutputMerger::CreateGBufferTextures(unsigned int client_width, unsigned int client_height)
{
	ID3D11Texture2D* gbuffer_position = CreateGBufferTexture(DXGI_FORMAT_R32G32B32A32_FLOAT, client_width, client_height);
	d3d11_gbuffers_.emplace_back(gbuffer_position);
	ID3D11Texture2D* gbuffer_normal = CreateGBufferTexture(DXGI_FORMAT_R16G16B16A16_FLOAT, client_width, client_height);
	d3d11_gbuffers_.emplace_back(gbuffer_normal);
	ID3D11Texture2D* gbuffer_diffuse = CreateGBufferTexture(DXGI_FORMAT_R8G8B8A8_UNORM, client_width, client_height);
	d3d11_gbuffers_.emplace_back(gbuffer_diffuse);
	ID3D11Texture2D* gbuffer_view_direction = CreateGBufferTexture(DXGI_FORMAT_R8G8B8A8_UNORM, client_width, client_height);
	d3d11_gbuffers_.emplace_back(gbuffer_view_direction);
}

void OutputMerger::CreateRenderTargetViewAndShaderResourceView(ID3D11Texture2D* texture, ID3D11RenderTargetView** render_target_view, ID3D11ShaderResourceView** shader_resource_view)
{
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateRenderTargetView(texture, nullptr, render_target_view);
	if (result != S_OK) 
	{
		throw std::string("Can Not Create GBuffer Render Target View");
	}
	 
	result = DeviceManager::GetInstance()->GetD3D11Device()->CreateShaderResourceView(texture, nullptr, shader_resource_view);
	if (result != S_OK) 
	{
		throw std::string("Can Not Create GBuffer Shader Resource View");
	}
}

void OutputMerger::CreateRenderTargetViewsAndShaderResourceViews()
{
	ID3D11RenderTargetView* gbuffer_render_target_view_position;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_position;
	CreateRenderTargetViewAndShaderResourceView(d3d11_gbuffers_[0].Get(), &gbuffer_render_target_view_position, &gbuffer_shader_resource_view_position);
	d3d11_gbuffer_render_target_views_.emplace_back(gbuffer_render_target_view_position);
	d3d11_gbuffer_shader_resource_views_.emplace_back(gbuffer_shader_resource_view_position);

	ID3D11RenderTargetView* gbuffer_render_target_view_normal;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_normal;
	CreateRenderTargetViewAndShaderResourceView(d3d11_gbuffers_[1].Get(), &gbuffer_render_target_view_normal, &gbuffer_shader_resource_view_normal);
	d3d11_gbuffer_render_target_views_.emplace_back(gbuffer_render_target_view_normal);
	d3d11_gbuffer_shader_resource_views_.emplace_back(gbuffer_shader_resource_view_normal);

	ID3D11RenderTargetView* gbuffer_render_target_view_diffuse;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_diffuse;
	CreateRenderTargetViewAndShaderResourceView(d3d11_gbuffers_[2].Get(), &gbuffer_render_target_view_diffuse, &gbuffer_shader_resource_view_diffuse);
	d3d11_gbuffer_render_target_views_.emplace_back(gbuffer_render_target_view_diffuse);
	d3d11_gbuffer_shader_resource_views_.emplace_back(gbuffer_shader_resource_view_diffuse);

	ID3D11RenderTargetView* gbuffer_render_target_view_view_direction;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_view_direction;
	CreateRenderTargetViewAndShaderResourceView(d3d11_gbuffers_[3].Get(), &gbuffer_render_target_view_view_direction, &gbuffer_shader_resource_view_view_direction);
	d3d11_gbuffer_render_target_views_.emplace_back(gbuffer_render_target_view_view_direction);
	d3d11_gbuffer_shader_resource_views_.emplace_back(gbuffer_shader_resource_view_view_direction);
}

void OutputMerger::CreateBackBufferView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	SwapChainManager::GetInstance()->GetDXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateRenderTargetView(back_buffer.Get(), 0, d3d11_render_target_view_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Render Target View");
	}
}

void OutputMerger::CreateRenderTargetViews(unsigned int client_width, unsigned int client_height)
{
	CreateGBufferTextures(client_width, client_height);
	CreateRenderTargetViewsAndShaderResourceViews();
	CreateBackBufferView();
}

void OutputMerger::CreateDepthStencilView(unsigned int client_width, unsigned int client_height)
{
	D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc;
	depth_stencil_buffer_desc.Width = client_width;
	depth_stencil_buffer_desc.Height = client_height;
	depth_stencil_buffer_desc.MipLevels = 1;
	depth_stencil_buffer_desc.ArraySize = 1;
	depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_buffer_desc.SampleDesc.Count = 1;
	depth_stencil_buffer_desc.SampleDesc.Quality = 0;
	depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_buffer_desc.CPUAccessFlags = 0;
	depth_stencil_buffer_desc.MiscFlags = 0;

	DeviceManager::GetInstance()->GetD3D11Device()->CreateTexture2D(&depth_stencil_buffer_desc, nullptr, d3d11_depth_stencil_buffer_.GetAddressOf());
	
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilView(d3d11_depth_stencil_buffer_.Get(), nullptr, d3d11_depth_stencil_view_.GetAddressOf());

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

void OutputMerger::CreateSecondPassDepthStencilState()
{
	// 깊이 스텐실 상태 설명
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depth_stencil_desc.DepthEnable = FALSE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS; // 이 설정은 무시되지만 기본값으로 설정

	// 스텐실 테스트 활성화
	depth_stencil_desc.StencilEnable = FALSE; // TEST 깊이 스텐실 정독하기
	depth_stencil_desc.StencilReadMask = 0xFF;       // 모든 비트 읽기 허용
	depth_stencil_desc.StencilWriteMask = 0xFF;       
	 
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// 깊이 스텐실 상태 객체 생성
	ID3D11DepthStencilState* depth_stencil_state = nullptr;
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);
	 
	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil State");
	}

	d3d11_depth_stencil_state_container_["SecondPass"] = depth_stencil_state;
}

void OutputMerger::CreateDepthStencilStates()
{
	CreateSecondPassDepthStencilState();
}

void OutputMerger::Initialize(unsigned int client_width, unsigned int client_height)
{
	CreateRenderTargetViews(client_width, client_height);
	CreateDepthStencilView(client_width, client_height);
	CreateViewPort(client_width, client_height);
	CreateDepthStencilStates();
}

ID3D11RenderTargetView* OutputMerger::GetGBufferPositionRenderTargetView() const
{
	return d3d11_gbuffer_render_target_views_[0].Get();
}

ID3D11RenderTargetView* OutputMerger::GetGBufferNormalRenderTargetView() const
{
	return d3d11_gbuffer_render_target_views_[1].Get();
}

ID3D11RenderTargetView* OutputMerger::GetGBufferDiffuseRenderTargetView() const
{
	return d3d11_gbuffer_render_target_views_[2].Get();
}

ID3D11RenderTargetView* OutputMerger::GetGBufferViewDirectionRenderTargetView() const
{
	return d3d11_gbuffer_render_target_views_[3].Get();
}

ID3D11ShaderResourceView* OutputMerger::GetGBufferPositionShaderResourceView() const
{
	return d3d11_gbuffer_shader_resource_views_[0].Get();
}

ID3D11ShaderResourceView* OutputMerger::GetGBufferNormalShaderResourceView() const
{
	return d3d11_gbuffer_shader_resource_views_[1].Get();
}

ID3D11ShaderResourceView* OutputMerger::GetGBufferDiffuseShaderResourceView() const
{
	return d3d11_gbuffer_shader_resource_views_[2].Get();
}

ID3D11ShaderResourceView* OutputMerger::GetGBufferViewDirectionShaderResourceView() const
{
	return d3d11_gbuffer_shader_resource_views_[3].Get();
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

ID3D11DepthStencilState* OutputMerger::GetDepthStencilState(std::string state_name) 
{
	if (d3d11_depth_stencil_state_container_.find(state_name) != d3d11_depth_stencil_state_container_.end())
	{
		return d3d11_depth_stencil_state_container_[state_name].Get();
	}

	throw std::string("Depth Stencil State Name Error");
}