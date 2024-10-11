#include "App.h"

App::App() : window_width_(NULL), window_height_(NULL), hwnd_(NULL)
{
	swap_chain_ = std::make_unique<SwapChain>();
	depth_stencil_view_ = std::make_unique<DepthStencilView>();
}

void App::Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd)
{
	window_width_ = window_width;
	window_height_ = window_height;
	hwnd_ = hwnd;

	Device::GetInstace()->Initialize();
	swap_chain_->Initialize(window_width_, window_height_, hwnd_);
	depth_stencil_view_->Initialize(window_width_, window_height_);
} 