#pragma once
#include "framework.h"

//TEST
#include "FrameResources/OutputMerger.h"

class App
{
private:
	unsigned int window_width_;
	unsigned int window_height_;
	HWND hwnd_;

public:
	App();

private:
	//std::unique_ptr<Device> device_; //TEST Singleton
	//std::unique_ptr<SwapChain> swap_chain_; //TEST Singleton
	std::unique_ptr<OutputMerger> output_merger_; //TEST

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd);
};

