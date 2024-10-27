#pragma once
#include <wrl.h>

class SwapChainManager
{
private:
	static SwapChainManager* instance_;

	SwapChainManager() = default;
	~SwapChainManager() = default;

public:
	SwapChainManager(const SwapChainManager&) = delete;
	SwapChainManager& operator=(const SwapChainManager&) = delete;

	static SwapChainManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new SwapChainManager();
		}

		return instance_;
	}

private:
	Microsoft::WRL::ComPtr<class IDXGISwapChain> d3d11_swap_chain_; 

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd); 
	class IDXGISwapChain* GetDXGISwapChain() const;
};

