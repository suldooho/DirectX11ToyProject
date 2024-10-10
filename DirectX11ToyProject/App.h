#pragma once
#include "framework.h"
#include "Core/SwapChain.h"
#include "Core/Device.h"

class App
{
private:
	unsigned int window_width_;
	unsigned int window_height_;
	HWND hwnd_;

public:
	App();

private:
	//std::unique_ptr<class Device> device_; //TEST Singleton
	std::unique_ptr<SwapChain> swap_chain_;

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd);
};

