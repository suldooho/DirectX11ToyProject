#include "ObjectsManager.h"
#include "../framework.h"
#include "SkySphereObject.h"
#include "FloorObject.h"
#include "../FrameResources/OutputMerger.h"
#include "DeferredRenderingSecondPass.h"

ObjectsManager* ObjectsManager::instance_ = nullptr;

void ObjectsManager::CreateObjects()
{
	SkySphereObject* sky_sphere_object = new SkySphereObject();
	sky_sphere_object->Initialize();
	objects_.emplace_back(sky_sphere_object);

	FloorObject* floor_object = new FloorObject();
	floor_object->Initialize();
	objects_.emplace_back(floor_object);
}

void ObjectsManager::ExecuteCommandListPlayer()
{
	camera_->UpdateViewMatrix();
	camera_->UpdateConstantBuffer();

	player_->UpdateBuffer();
	player_->ExecuteCommandList();
}

void ObjectsManager::ExecuteCommandListObjects()
{
	for (unsigned int i = 0; i < objects_.size(); ++i)
	{
		objects_[i].get()->UpdateBuffer();
		objects_[i].get()->ExecuteCommandList();
	}
}

void ObjectsManager::ExecuteCommandSecondPass()
{
	deferred_rendering_second_pass_->ExecuteCommandList();
}

void ObjectsManager::ClearRenderTargetViewAndDepthStencilView()
{
	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	float clear_color[4] = { 0.0f,  0.0f,  0.0f, 0.0f };
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearRenderTargetView(output_merger->GetRenderTargetView("GBufferPositionView"), clear_color);
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearRenderTargetView(output_merger->GetRenderTargetView("GBufferNormalView"), clear_color);
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearRenderTargetView(output_merger->GetRenderTargetView("GBufferDiffuseView"), clear_color);
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearRenderTargetView(output_merger->GetRenderTargetView("GBufferViewDirectionView"), clear_color);
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearRenderTargetView(output_merger->GetRenderTargetView("BackBufferView"), clear_color);
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearDepthStencilView(output_merger->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f); 
}

void ObjectsManager::MoveCamera(float delta_time)
{
	DirectX::XMVECTOR move_vector = DirectX::XMVectorZero();
	 
	if (camera_direction_ & kAKey_)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, DirectX::XMVectorScale(camera_->GetRight(), -1.0f));
	}
	if (camera_direction_ & kDKey_)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, camera_->GetRight());
	}
	if (camera_direction_ & kWKey_)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, camera_->GetLook());
	}
	if (camera_direction_ & kSKey_)
	{
		move_vector = DirectX::XMVectorAdd(move_vector, DirectX::XMVectorScale(camera_->GetLook(), -1.0f));
	}
	
	if (!DirectX::XMVector3Equal(move_vector, DirectX::XMVectorZero()))
	{
		move_vector = DirectX::XMVectorSet(DirectX::XMVectorGetX(move_vector), 0.0f, DirectX::XMVectorGetZ(move_vector), 0.0f);
		DirectX::XMVECTOR normalized_move_vector = DirectX::XMVector3Normalize(move_vector);
		DirectX::XMVECTOR delta_time_move_vector = DirectX::XMVectorScale(normalized_move_vector, delta_time);
		 
		camera_->Move(delta_time_move_vector);
	}
}

void ObjectsManager::RotateCamera(float delta_time)
{ 
	camera_->RotateYaw(camera_yaw_ * delta_time);
	camera_->RotatePitch(camera_pitch_ * delta_time);

	camera_yaw_ = 0.0f;
	camera_pitch_ = 0.0f;
}

void ObjectsManager::SetPlayerRotationAndPosition(DirectX::FXMMATRIX camera_world_matrix)
{
	player_->SetRotationAndPosition(camera_world_matrix);
}

void ObjectsManager::Initialize(float client_width, float client_height)
{
	camera_direction_ = 0;
	camera_yaw_ = 0.0f;
	camera_pitch_ = 0.0f;

	camera_ = std::make_unique<Camera>();
	camera_->Initialize(client_width, client_height);

	player_ = std::make_unique<Player>();
	player_->Initialize(); 

	deferred_rendering_second_pass_ = std::make_unique<DeferredRenderingSecondPass>();
	deferred_rendering_second_pass_->Initialize();

	CreateObjects();
}

void ObjectsManager::PushButton(unsigned int direction)
{
	camera_direction_ |= direction;
}

void ObjectsManager::ReleaseButton(unsigned int direction)
{
	camera_direction_ &= ~direction;
}

void ObjectsManager::SetRotationValue(float yaw, float pitch)
{
	camera_yaw_ = yaw *  kRotationSensitivity_;
	camera_pitch_ = pitch * kRotationSensitivity_;
} 

ID3D11Buffer** ObjectsManager::GetCameraConstantBuffer() const
{
	return camera_->GetCameraConstantBuffer();
}

void ObjectsManager::AnimateObjects()
{
	float delta_time = TimerManager::GetInstance()->GetDeltaTime();
	 
	MoveCamera(delta_time);
	RotateCamera(delta_time);
	SetPlayerRotationAndPosition(camera_->GetWorldMatrix());
}

void ObjectsManager::ExecuteCommandList()
{  
	ClearRenderTargetViewAndDepthStencilView();

	ExecuteCommandListPlayer();
	ExecuteCommandListObjects();
	ExecuteCommandSecondPass();

	SwapChainManager::GetInstance()->GetDXGISwapChain()->Present(0, 0);
}
