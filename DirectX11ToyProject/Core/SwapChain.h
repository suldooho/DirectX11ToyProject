#pragma once
#include <wrl.h>

class SwapChain
{
private:
	Microsoft::WRL::ComPtr<class IDXGISwapChain> d3d11_swap_chain_;

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd);
};

