#pragma once
#include "FrameResource.h"
#include <wrl.h> 
#include <memory> 
#include <unordered_map>

class OutputMerger : public FrameResource
{ 
private:
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11Texture2D>> gbuffer_container_; // position normal diffuse view_direction   
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11RenderTargetView>> render_target_view_container_; // position normal diffuse view_direction  
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView>> shader_resource_view_container_; // position normal diffuse view_direction  
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11DepthStencilState>> depth_stencil_state_container_;

	Microsoft::WRL::ComPtr<class ID3D11Texture2D> depth_stencil_buffer_;  
	Microsoft::WRL::ComPtr<class ID3D11DepthStencilView> depth_stencil_view_;

	std::unique_ptr<class D3D11_VIEWPORT> viewport_;
	 
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
	class ID3D11Texture2D* GetGbuffer(std::string gbuffer_name);
	class ID3D11RenderTargetView* GetRenderTargetView(std::string view_name);
	class ID3D11ShaderResourceView* GetShaderResourceView(std::string view_name);
	class ID3D11DepthStencilState* GetDepthStencilState(std::string state_name);
	 
	class ID3D11DepthStencilView* GetDepthStencilView() const;
	class D3D11_VIEWPORT* GetViewport() const;
};

