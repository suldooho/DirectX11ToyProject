#include "App.h"
#include "Objects/ObjectsManager.h"

App::App() : window_width_(NULL), window_height_(NULL), hwnd_(NULL)
{  
}

void App::Initialize(unsigned int window_width, unsigned int window_height, HWND hwnd)
{
	window_width_ = window_width;
	window_height_ = window_height;
	hwnd_ = hwnd;

	TimerManager::GetInstace()->Initialize();
	DeviceManager::GetInstace()->Initialize();
	SwapChainManager::GetInstace()->Initialize(window_width_, window_height_, hwnd_);
	FrameResourcesManager::GetInstace()->Initialize(window_width_, window_height_);
	MeshesManager::GetInstace()->Initialize();
	ObjectsManager::GetInstace()->Initialize(static_cast<float>(window_width_), static_cast<float>(window_height_));
}

void App::OnProcessingMouseMessage(HWND hwnd, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	static POINT last_mouse_pos = GetWindowCenter(); 
	POINT current_mouse_pos;

	switch (message_id)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE: 
		GetCursorPos(&current_mouse_pos);  

		if (current_mouse_pos.x != last_mouse_pos.x || current_mouse_pos.y != last_mouse_pos.y)
		{
			DebugLog::GetInstace()->LogFormattedMessage("%d, %d, %d, %d\n", current_mouse_pos.x, last_mouse_pos.x, current_mouse_pos.y, last_mouse_pos.y);
			ObjectsManager::GetInstace()->SetRotationValue(static_cast<float>(current_mouse_pos.x - last_mouse_pos.x), static_cast<float>(-(current_mouse_pos.y - last_mouse_pos.y)));
		}

		POINT center = GetWindowCenter();
		ClientToScreen(hwnd, &center);
		SetCursorPos(center.x, center.y);
		 
		last_mouse_pos = center;
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
		case 0x41:  // A key
			ObjectsManager::GetInstace()->PushButton(ObjectsManager::GetInstace()->kAKey);
			break;
		case 0x53:  // S key
			ObjectsManager::GetInstace()->PushButton(ObjectsManager::GetInstace()->kSKey);
			break;
		case 0x44:  // D key
			ObjectsManager::GetInstace()->PushButton(ObjectsManager::GetInstace()->kDKey);
			break;
		case 0x57:  // W key
			ObjectsManager::GetInstace()->PushButton(ObjectsManager::GetInstace()->kWKey);
			break; 
		default:
			break;
		} 
		break;
	case WM_KEYUP:
		switch (wparam)
		{
		case 0x41:  // A key
			ObjectsManager::GetInstace()->ReleaseButton(ObjectsManager::GetInstace()->kAKey);
			break;
		case 0x53:  // S key
			ObjectsManager::GetInstace()->ReleaseButton(ObjectsManager::GetInstace()->kSKey);
			break;
		case 0x44:  // D key
			ObjectsManager::GetInstace()->ReleaseButton(ObjectsManager::GetInstace()->kDKey);
			break;
		case 0x57:  // W key
			ObjectsManager::GetInstace()->ReleaseButton(ObjectsManager::GetInstace()->kWKey);
			break;
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

void App::FrameAdvance()
{
	TimerManager::GetInstace()->Tick();
	ObjectsManager::GetInstace()->AnimateObjects();
	ObjectsManager::GetInstace()->ExecuteCommandList();
}

POINT App::GetWindowCenter()
{ 
	POINT center;
	center.x = window_width_ / 2;
	center.y = window_height_ / 2;

	return center;
}
