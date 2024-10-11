#include "App.h"

App::App() : window_width_(NULL), window_height_(NULL), hwnd_(NULL)
{ 
	output_merger_ = std::make_unique<OutputMerger>();
}

void App::Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd)
{
	window_width_ = window_width;
	window_height_ = window_height;
	hwnd_ = hwnd;

	Device::GetInstace()->Initialize();
	SwapChain::GetInstace()->Initialize(window_width_, window_height_, hwnd); 
	output_merger_->Initialize(window_width_, window_height_);
} 