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
	gbuffer_container_["PositionBuffer"] = gbuffer_position; 
	ID3D11Texture2D* gbuffer_normal = CreateGBufferTexture(DXGI_FORMAT_R32G32B32A32_FLOAT, client_width, client_height);
	gbuffer_container_["NormalBuffer"] = gbuffer_normal;
	ID3D11Texture2D* gbuffer_diffuse = CreateGBufferTexture(DXGI_FORMAT_R8G8B8A8_UNORM, client_width, client_height);
	gbuffer_container_["DiffuseBuffer"] = gbuffer_diffuse;
	ID3D11Texture2D* gbuffer_view_direction = CreateGBufferTexture(DXGI_FORMAT_R8G8B8A8_UNORM, client_width, client_height);
	gbuffer_container_["ViewDirectionBuffer"] = gbuffer_view_direction;
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
	CreateRenderTargetViewAndShaderResourceView(gbuffer_container_["PositionBuffer"].Get(), &gbuffer_render_target_view_position, &gbuffer_shader_resource_view_position);
	render_target_view_container_["GBufferPositionView"] = gbuffer_render_target_view_position;
	shader_resource_view_container_["GBufferPositionView"] = gbuffer_shader_resource_view_position; 

	ID3D11RenderTargetView* gbuffer_render_target_view_normal;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_normal;
	CreateRenderTargetViewAndShaderResourceView(gbuffer_container_["NormalBuffer"].Get(), &gbuffer_render_target_view_normal, &gbuffer_shader_resource_view_normal);
	render_target_view_container_["GBufferNormalView"] = gbuffer_render_target_view_normal;
	shader_resource_view_container_["GBufferNormalView"] = gbuffer_shader_resource_view_normal;

	ID3D11RenderTargetView* gbuffer_render_target_view_diffuse;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_diffuse;
	CreateRenderTargetViewAndShaderResourceView(gbuffer_container_["DiffuseBuffer"].Get(), &gbuffer_render_target_view_diffuse, &gbuffer_shader_resource_view_diffuse);
	render_target_view_container_["GBufferDiffuseView"] = gbuffer_render_target_view_diffuse;
	shader_resource_view_container_["GBufferDiffuseView"] = gbuffer_shader_resource_view_diffuse;

	ID3D11RenderTargetView* gbuffer_render_target_view_view_direction;
	ID3D11ShaderResourceView* gbuffer_shader_resource_view_view_direction;
	CreateRenderTargetViewAndShaderResourceView(gbuffer_container_["ViewDirectionBuffer"].Get(), &gbuffer_render_target_view_view_direction, &gbuffer_shader_resource_view_view_direction);
	render_target_view_container_["GBufferViewDirectionView"] = gbuffer_render_target_view_view_direction;
	shader_resource_view_container_["GBufferViewDirectionView"] = gbuffer_shader_resource_view_view_direction;
}

void OutputMerger::CreateBackBufferView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	SwapChainManager::GetInstance()->GetDXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
	ID3D11RenderTargetView* render_target_view = nullptr;
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateRenderTargetView(back_buffer.Get(), 0, &render_target_view);

	if (result != S_OK)
	{
		throw std::string("Can Not Create Back Buffer View");
	}

	render_target_view_container_["BackBufferView"] = render_target_view;	
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

	DeviceManager::GetInstance()->GetD3D11Device()->CreateTexture2D(&depth_stencil_buffer_desc, nullptr, depth_stencil_buffer_.GetAddressOf());
	 
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilView(depth_stencil_buffer_.Get(), nullptr, depth_stencil_view_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil View");
	}
}

void OutputMerger::CreateViewPort(unsigned int client_width, unsigned int client_height)
{
	viewport_ = std::make_unique<D3D11_VIEWPORT>();
	viewport_.get()->TopLeftX = 0.0f;
	viewport_.get()->TopLeftY = 0.0f;
	viewport_.get()->Width = static_cast<float>(client_width);
	viewport_.get()->Height = static_cast<float>(client_height);
	viewport_.get()->MinDepth = 0.0f;
	viewport_.get()->MaxDepth = 1.0f;
}
 
void OutputMerger::CreateDepthStencilStates()
{
	// ±íÀÌ ½ºÅÙ½Ç »óÅÂ ¼³¸í
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depth_stencil_desc.DepthEnable = FALSE;
	depth_stencil_desc.StencilEnable = FALSE;

	ID3D11DepthStencilState* depth_stencil_state = nullptr;
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);

	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil State");
	}
	depth_stencil_state_container_["SecondPassState"] = depth_stencil_state;


	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc2;
	ZeroMemory(&depth_stencil_desc2, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depth_stencil_desc2.DepthEnable = true;
	depth_stencil_desc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc2.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc2.StencilEnable = true;
	depth_stencil_desc2.StencilReadMask = 0xff;
	depth_stencil_desc2.StencilWriteMask = 0xff;

	depth_stencil_desc2.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	depth_stencil_desc2.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	depth_stencil_desc2.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc2.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depth_stencil_desc2.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc2.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	depth_stencil_desc2.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc2.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// ±íÀÌ ½ºÅÙ½Ç »óÅÂ °´Ã¼ »ý¼º
	ID3D11DepthStencilState* depth_stencil_state2 = nullptr;
	result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilState(&depth_stencil_desc2, &depth_stencil_state2);

	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil State");
	}
	depth_stencil_state_container_["FirstPassState"] = depth_stencil_state2;


	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc3;
	ZeroMemory(&depth_stencil_desc3, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depth_stencil_desc3.DepthEnable = true;
	depth_stencil_desc3.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc3.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc3.StencilEnable = true;
	depth_stencil_desc3.StencilReadMask = 0xff;
	depth_stencil_desc3.StencilWriteMask = 0xff;

	depth_stencil_desc3.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depth_stencil_desc3.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc3.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc3.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depth_stencil_desc3.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	depth_stencil_desc3.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc3.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc3.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// ±íÀÌ ½ºÅÙ½Ç »óÅÂ °´Ã¼ »ý¼º
	ID3D11DepthStencilState* depth_stencil_state3 = nullptr;
	result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilState(&depth_stencil_desc3, &depth_stencil_state3);

	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil State");
	}
	depth_stencil_state_container_["ForwardPassState"] = depth_stencil_state3;


	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc4;
	ZeroMemory(&depth_stencil_desc4, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depth_stencil_desc4.DepthEnable = true;
	depth_stencil_desc4.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc4.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc4.StencilEnable = true;
	depth_stencil_desc4.StencilReadMask = 0xff;
	depth_stencil_desc4.StencilWriteMask = 0xff;

	depth_stencil_desc4.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc4.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	depth_stencil_desc4.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc4.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depth_stencil_desc4.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	depth_stencil_desc4.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc4.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc4.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// ±íÀÌ ½ºÅÙ½Ç »óÅÂ °´Ã¼ »ý¼º
	ID3D11DepthStencilState* depth_stencil_state4 = nullptr;
	result = DeviceManager::GetInstance()->GetD3D11Device()->CreateDepthStencilState(&depth_stencil_desc4, &depth_stencil_state4);

	if (result != S_OK)
	{
		throw std::string("Can Not Create Depth Stencil State");
	}
	depth_stencil_state_container_["FloorObjectState"] = depth_stencil_state4;
}

void OutputMerger::CreateBlendStates()
{
	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* blend_state = nullptr;
	HRESULT result = DeviceManager::GetInstance()->GetD3D11Device()->CreateBlendState(&blend_desc, &blend_state);

	if (result != S_OK)
	{
		throw std::string("Can Not Blend State");
	}
	blend_state_container_["Transparent"] = blend_state; 

}

void OutputMerger::Initialize(unsigned int client_width, unsigned int client_height)
{
	CreateRenderTargetViews(client_width, client_height);
	CreateDepthStencilView(client_width, client_height);
	CreateViewPort(client_width, client_height);
	CreateDepthStencilStates();
	CreateBlendStates();
} 


ID3D11Texture2D* OutputMerger::GetGbuffer(std::string gbuffer_name)
{
	if (gbuffer_container_.find(gbuffer_name) != gbuffer_container_.end())
	{
		return gbuffer_container_[gbuffer_name].Get();
	}

	throw std::string("Gbuffer Name Error");
}

ID3D11RenderTargetView* OutputMerger::GetRenderTargetView(std::string view_name)
{
	if (render_target_view_container_.find(view_name) != render_target_view_container_.end())
	{
		return render_target_view_container_[view_name].Get();
	}

	throw std::string("Render Target View Name Error");
}

ID3D11ShaderResourceView* OutputMerger::GetShaderResourceView(std::string view_name)
{
	if (shader_resource_view_container_.find(view_name) != shader_resource_view_container_.end())
	{
		return shader_resource_view_container_[view_name].Get();
	}

	throw std::string("Shader Resource View Name Error");
}

ID3D11DepthStencilState* OutputMerger::GetDepthStencilState(std::string state_name)
{
	if (depth_stencil_state_container_.find(state_name) != depth_stencil_state_container_.end())
	{
		return depth_stencil_state_container_[state_name].Get();
	}

	throw std::string("Depth Stencil State Name Error");
}

ID3D11BlendState* OutputMerger::GetBlendState(std::string state_name)
{
	if (blend_state_container_.find(state_name) != blend_state_container_.end())
	{
		return blend_state_container_[state_name].Get();
	}

	throw std::string("Blend State Name Error");
}

ID3D11DepthStencilView* OutputMerger::GetDepthStencilView() const
{
	return depth_stencil_view_.Get();
}

D3D11_VIEWPORT* OutputMerger::GetViewport() const
{
	return viewport_.get();
}