#include "ObjectsManager.h"
#include "../framework.h"

ObjectsManager* ObjectsManager::instance_ = nullptr;

void ObjectsManager::Initialize(float client_width, float client_height)
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(client_width, client_height);

	player_ = std::make_unique<Player>();
	player_->Initialize();

	camera_->SetPlayer(player_.get());
}

ID3D11Buffer** ObjectsManager::GetAddressOfCameraConstantBuffer() const
{
	return camera_->GetAddressOfCameraConstantBuffer();
}

void ObjectsManager::ExecuteCommandList()
{ 
	camera_->UpdateViewMatrix();

	camera_->UpdateConstantBuffer();

	player_->UpdateConstantBuffer();
	player_->ExecuteCommandList(); 

	SwapChainManager::GetInstace()->GetDXGISwapChain()->Present(0, 0);
}
