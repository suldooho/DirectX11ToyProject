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

	float clear_color[4] = { 0.125f,  0.125f,  0.125f, 1.0f };
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->ClearRenderTargetView(output_merger->GetRenderTargetView(), clear_color);
	DeviceManager::GetInstace()->GetD3D11ImmediateContext()->ClearDepthStencilView(output_merger->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}

void ObjectsManager::MovePlayer(float delta_time)
{
	DirectX::XMVECTOR move_vector = DirectX::XMVectorZero();
	 
	if (player_direction_ & kAKey)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));  
	}
	if (player_direction_ & kDKey)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));  
	}
	if (player_direction_ & kWKey)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));  
	}
	if (player_direction_ & kSKey)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));  
	}
	
	if (!DirectX::XMVector3Equal(move_vector, DirectX::XMVectorZero()))
	{
		DirectX::XMVECTOR normalized_move_vector = DirectX::XMVector3Normalize(move_vector);
		DirectX::XMVECTOR delta_time_move_vector = DirectX::XMVectorScale(normalized_move_vector, delta_time);

		player_->Move(delta_time_move_vector);
		camera_->SetPosition(player_->GetPosition());
	}
}

void ObjectsManager::RotatePlayer(float delta_time)
{ 
	camera_->RotateYaw(player_yaw_ * delta_time);
	
	player_yaw_ = 0.0f;// TEST
}

void ObjectsManager::Initialize(float client_width, float client_height)
{
	player_direction_ = 0;
	player_yaw_ = 0.0f;
	player_pitch_ = 0.0f;

	camera_ = std::make_unique<Camera>();
	camera_->Initialize(client_width, client_height);

	player_ = std::make_unique<Player>();
	player_->Initialize();

	camera_->SetPlayer(player_.get());

	CreateMovableObjects();
}

void ObjectsManager::PushButton(unsigned int direction)
{
	player_direction_ |= direction;
}

void ObjectsManager::ReleaseButton(unsigned int direction)
{
	player_direction_ &= ~direction;
}

void ObjectsManager::SetRotationValue(float yaw, float pitch)
{
	player_yaw_ = yaw * 0.1f;
	player_pitch_ = pitch * 0.1f; // TEST
} 

ID3D11Buffer** ObjectsManager::GetAddressOfCameraConstantBuffer() const
{
	return camera_->GetAddressOfCameraConstantBuffer();
}

void ObjectsManager::AnimateObjects()
{
	float delta_time = TimerManager::GetInstace()->DeltaTime();
	 
	MovePlayer(delta_time);
	RotatePlayer(delta_time);
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
