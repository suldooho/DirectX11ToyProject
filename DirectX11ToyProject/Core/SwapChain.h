#pragma once
#include <wrl.h>

class SwapChain
{
private:
	Microsoft::WRL::ComPtr<class IDXGISwapChain> d3d11_swap_chain_;
	Microsoft::WRL::ComPtr<class ID3D11RenderTargetView> d3d11_render_target_view_;

private:
	void CreateRenderTargetView();

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd);
};

