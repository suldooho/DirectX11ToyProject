#pragma once
#include "framework.h"

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

public:
	void Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd);
	void OnProcessingMouseMessage(HWND hwnd, UINT message_id, WPARAM wparam, LPARAM lparam);
	void OnProcessingKeyboardMessage(HWND hwnd, UINT message_id, WPARAM wparam, LPARAM lparam);
};

