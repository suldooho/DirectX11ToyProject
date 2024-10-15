#include "App.h"

App::App() : window_width_(NULL), window_height_(NULL), hwnd_(NULL)
{  
}

void App::Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd)
{
	window_width_ = window_width;
	window_height_ = window_height;
	hwnd_ = hwnd;

	DeviceManager::GetInstace()->Initialize();
	SwapChainManager::GetInstace()->Initialize(window_width_, window_height_, hwnd_);
	FrameResourcesManager::GetInstace()->Initialize(window_width_, window_height_);
	MeshesManager::GetInstace()->Initialize();
}

void App::OnProcessingMouseMessage(HWND hwnd, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void App::OnProcessingKeyboardMessage(HWND hwnd, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_KEYDOWN:
		switch (wparam)
		{

		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch (wparam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		default:
			break;
		} 
		break;
	default:
		break;
	}
}
