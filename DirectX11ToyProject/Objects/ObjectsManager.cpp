#include "ObjectsManager.h"
#include "../framework.h"
#include "BoxObject.h"
#include "../FrameResources/OutputMerger.h"

ObjectsManager* ObjectsManager::instance_ = nullptr;

void ObjectsManager::CreateMovableObjects()
{
	BoxObject* box_object = new BoxObject();
	box_object->Initialize();
	movable_objects_.emplace_back(box_object);
}

void ObjectsManager::ExecuteCommandListMovableObjects()
{
	for (unsigned int i = 0; i < movable_objects_.size(); ++i)
	{
		movable_objects_[i].get()->UpdateConstantBuffer();
		movable_objects_[i].get()->ExecuteCommandList();
	}
}

void ObjectsManager::ClearDepthStencilView()
{
	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstace()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->ClearDepthStencilView(output_merger->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}

void ObjectsManager::Initialize(float client_width, float client_height)
{
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(client_width, client_height);

	player_ = std::make_unique<Player>();
	player_->Initialize();

	camera_->SetPlayer(player_.get());

	CreateMovableObjects();
}

ID3D11Buffer** ObjectsManager::GetAddressOfCameraConstantBuffer() const
{
	return camera_->GetAddressOfCameraConstantBuffer();
}

void ObjectsManager::ExecuteCommandList()
{  
	ClearDepthStencilView();

	camera_->UpdateViewMatrix();

	camera_->UpdateConstantBuffer();

	player_->UpdateConstantBuffer();
	player_->ExecuteCommandList(); 

	ExecuteCommandListMovableObjects();

	SwapChainManager::GetInstace()->GetDXGISwapChain()->Present(0, 0);
}
