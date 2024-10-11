#pragma once
#include <wrl.h>

class SwapChain
{
private:
	static SwapChain* instance_;

	SwapChain() = default;
	~SwapChain() = default;

public:
	SwapChain(const SwapChain&) = delete;
	SwapChain& operator=(const SwapChain&) = delete;

	static SwapChain* GetInstace()
	{
		if (instance_ == nullptr)
		{
			instance_ = new SwapChain();
		}

		return instance_;
	}

private:
	Microsoft::WRL::ComPtr<class IDXGISwapChain> d3d11_swap_chain_; 

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd); 
	class IDXGISwapChain* GetDXGISwapChain() const;
};

