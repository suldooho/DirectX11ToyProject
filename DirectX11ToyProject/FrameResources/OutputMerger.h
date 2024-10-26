#pragma once
#include "FrameResource.h"
#include <wrl.h> 
#include <memory>
#include <vector>
#include <unordered_map>

class OutputMerger : public FrameResource
{ 
private:
	std::vector<Microsoft::WRL::ComPtr<class ID3D11Texture2D>> d3d11_gbuffers_; // position normal diffuse view_direction  
	Microsoft::WRL::ComPtr<class ID3D11Texture2D> d3d11_depth_stencil_buffer_;
	 
	std::vector<Microsoft::WRL::ComPtr<class ID3D11RenderTargetView>> d3d11_gbuffer_render_target_views_; // position normal diffuse view_direction  
	std::vector<Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView>> d3d11_gbuffer_shader_resource_views_; // position normal diffuse view_direction  
	Microsoft::WRL::ComPtr<class ID3D11RenderTargetView> d3d11_render_target_view_;
	Microsoft::WRL::ComPtr<class ID3D11DepthStencilView> d3d11_depth_stencil_view_;

	std::unique_ptr<class D3D11_VIEWPORT> d3d11_viewport_;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11DepthStencilState>> d3d11_depth_stencil_state_container_; 

private:
	class ID3D11Texture2D* CreateGBufferTexture(enum DXGI_FORMAT format, unsigned int client_width, unsigned int client_height);
	void CreateGBufferTextures(unsigned int client_width, unsigned int client_height);
	void CreateRenderTargetViewAndShaderResourceView(class ID3D11Texture2D* texture, class ID3D11RenderTargetView** render_target_view, class ID3D11ShaderResourceView** shader_resource_view);
	void CreateRenderTargetViewsAndShaderResourceViews();
	void CreateBackBufferView();

private:
	void CreateRenderTargetViews(unsigned int client_width, unsigned int client_height);
	void CreateDepthStencilView(unsigned int client_width, unsigned int client_height);
	void CreateViewPort(unsigned int client_width, unsigned int client_height);

private:
	void CreateSecondPassDepthStencilState();
	void CreateDepthStencilStates();

public:
	void Initialize(unsigned int client_width, unsigned int client_height);

public:
	class ID3D11RenderTargetView* GetGBufferPositionRenderTargetView() const;
	class ID3D11RenderTargetView* GetGBufferNormalRenderTargetView() const;
	class ID3D11RenderTargetView* GetGBufferDiffuseRenderTargetView() const;
	class ID3D11RenderTargetView* GetGBufferViewDirectionRenderTargetView() const;

	class ID3D11ShaderResourceView* GetGBufferPositionShaderResourceView() const;
	class ID3D11ShaderResourceView* GetGBufferNormalShaderResourceView() const;
	class ID3D11ShaderResourceView* GetGBufferDiffuseShaderResourceView() const;
	class ID3D11ShaderResourceView* GetGBufferViewDirectionShaderResourceView() const;

	class ID3D11RenderTargetView* GetRenderTargetView() const;
	class ID3D11DepthStencilView* GetDepthStencilView() const;
	class D3D11_VIEWPORT* GetViewport() const;

	class ID3D11DepthStencilState* GetDepthStencilState(std::string state_name);
};

