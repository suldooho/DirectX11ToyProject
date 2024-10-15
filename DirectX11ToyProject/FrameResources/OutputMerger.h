#pragma once
#include "FrameResource.h"
#include <wrl.h> 
#include <memory>

class OutputMerger : public FrameResource
{ 
private:
	Microsoft::WRL::ComPtr<class ID3D11Texture2D> d3d11_depth_stencil_buffer_;

	Microsoft::WRL::ComPtr<class ID3D11RenderTargetView> d3d11_render_target_view_;
	Microsoft::WRL::ComPtr<class ID3D11DepthStencilView> d3d11_depth_stencil_view_;
	std::unique_ptr<class D3D11_VIEWPORT> d3d11_viewport_;

private:
	void CreateRenderTargetView();
	void CreateDepthStencilView(unsigned int client_width, unsigned int client_height);
	void CreateViewPort(unsigned int client_width, unsigned int client_height);

public:
	void Initialize(unsigned int client_width, unsigned int client_height);

public:
	class ID3D11RenderTargetView* GetRenderTargetView() const;
	class ID3D11DepthStencilView* GetDepthStencilView() const;
	class D3D11_VIEWPORT* GetViewport() const;
};

