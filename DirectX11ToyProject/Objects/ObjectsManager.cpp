#include "ObjectsManager.h"
#include "../framework.h"
#include "SkySphereObject.h"
#include "FloorObject.h"
#include "EnemyObject.h"
#include "BulletObject.h"
#include "../FrameResources/OutputMerger.h"
#include "DeferredRenderingSecondPass.h"
#include "../Meshes/Vertices.h" 
#include <random>

ObjectsManager* ObjectsManager::instance_ = nullptr;

void ObjectsManager::CreateObjects()
{
	SkySphereObject* sky_sphere_object = new SkySphereObject();
	sky_sphere_object->Initialize();
	deferred_rendering_objects_.emplace_back(sky_sphere_object);

	FloorObject* floor_object = new FloorObject();
	floor_object->Initialize();
	floor_object->TestInitialize();
	deferred_rendering_objects_.emplace_back(floor_object);

	BulletObject* bullet_object = new BulletObject();
	bullet_object->Initialize();
	forward_rendering_objects_.emplace_back(bullet_object);

	EnemyObject* enemy_object = new EnemyObject();
	enemy_object->Initialize();
	forward_rendering_objects_.emplace_back(enemy_object); 
}

void ObjectsManager::ExecuteCommandListPlayer()
{
	camera_->UpdateConstantBuffer();

	player_->UpdateBuffer();
	player_->ExecuteCommandList();
}

void ObjectsManager::DeferredRenderingExecuteCommandListObjects()
{
	for (unsigned int i = 0; i < deferred_rendering_objects_.size(); ++i)
	{
		deferred_rendering_objects_[i].get()->UpdateBuffer();
		deferred_rendering_objects_[i].get()->ExecuteCommandList();
	}
}

void ObjectsManager::DeferredRenderingTestExecuteCommandListObjects()
{
	deferred_rendering_objects_[0].get()->UpdateBuffer();
	deferred_rendering_objects_[0].get()->ExecuteCommandList();

	FloorObject* floor_object = dynamic_cast<FloorObject*>(deferred_rendering_objects_[1].get());
	floor_object->UpdateBuffer();
	floor_object->ExecuteTestCommandList();
}

void ObjectsManager::ForwardRenderingExecuteCommandListObjects()
{
	for (unsigned int i = 0; i < forward_rendering_objects_.size(); ++i)
	{
		forward_rendering_objects_[i].get()->UpdateBuffer();
		forward_rendering_objects_[i].get()->ExecuteCommandList();
	}
}

void ObjectsManager::DeferredRenderingExecuteCommandSecondPass()
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
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ClearDepthStencilView(output_merger->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); 
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

DirectX::XMMATRIX ObjectsManager::ScreenToWorld()
{
	float clip_x = (client_width_ / 2.0f) / client_width_ * 2.0f - 1.0f;
	float clip_y = 1.0f - (client_height_ / 2.0f) / client_height_ * 2.0f;
	DirectX::XMVECTOR ray_clip = DirectX::XMVectorSet(clip_x, clip_y, 0.00f, 1.0f); 
	 
	// 역투영 변환
	DirectX::XMMATRIX inv_view_projection = DirectX::XMMatrixInverse(nullptr, camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
	DirectX::XMVECTOR ray_world = DirectX::XMVector3TransformCoord(ray_clip, inv_view_projection);
	DirectX::XMMATRIX camera_world = camera_->GetWorldMatrix();

	DirectX::XMMATRIX bullet_world;
	bullet_world.r[0] = camera_world.r[0];
	bullet_world.r[1] = camera_world.r[1];
	bullet_world.r[2] = camera_world.r[2]; 


	DirectX::XMVECTOR right = DirectX::XMVectorScale(bullet_world.r[0], 1);
	ray_world = DirectX::XMVectorAdd(ray_world, right);
	DirectX::XMVECTOR up = DirectX::XMVectorScale(bullet_world.r[1], -0.5f);
	ray_world = DirectX::XMVectorAdd(ray_world, up);
	DirectX::XMVECTOR look = DirectX::XMVectorScale(bullet_world.r[2], 1.2f);
	ray_world = DirectX::XMVectorAdd(ray_world, look);

	bullet_world.r[3] = ray_world;

	return bullet_world;
}

void ObjectsManager::SetBulletPosition()
{ 
	static unsigned int bullet_count = 0; 

	if (mouse_click_)
	{ 
		BulletObject* bullet_objects = dynamic_cast<BulletObject*>(forward_rendering_objects_[0].get());
		if (bullet_objects == nullptr)
		{
			throw std::string("BulletObject dynamic_cast Fail");
		}

		click_elapsed_time_ += TimerManager::GetInstance()->GetDeltaTime();

		if (0.1f <= click_elapsed_time_)
		{ 
			for (unsigned int i = 0; i < bullet_objects->GetBulletCount(); ++i) // 비활성화된 총알을 찾는다.
			{
				if (!bullet_objects->GetActiveOfIndex(i))
				{
					DirectX::XMMATRIX new_bullet_world = ScreenToWorld();
					bullet_objects->SetActiveOfIndex(i);
					BulletInstanceData* bullet = bullet_objects->GetBulletDataOfIndex(i);
					 
					DirectX::XMStoreFloat3(&bullet->prevPosition, new_bullet_world.r[3]);
					DirectX::XMStoreFloat3(&bullet->position, new_bullet_world.r[3]);
					bullet_objects->SetInitPositionOfIndex(i, bullet->position); 
					DirectX::XMFLOAT3 camera_look;
					DirectX::XMStoreFloat3(&camera_look, camera_->GetLook());
					bullet_objects->SetInitDirectionOfIndex(i, camera_look);
					break;
				}
			}

			click_elapsed_time_ = 0.0;
			++bullet_count;
		}
		  
		if (bullet_count == 3)
		{
			mouse_click_ = false;
			bullet_count = 0;
			click_elapsed_time_ = 0.0; 
		}
	}
}

void ObjectsManager::Initialize(float client_width, float client_height)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));  

	test_ = false;

	client_width_ = static_cast<int>(client_width);
	client_height_ = static_cast<int>(client_height);

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

void ObjectsManager::ActiveTest()
{
	test_ = !test_;
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

void ObjectsManager::PushMouseLeftButton()
{
	mouse_click_ = true; 
	click_elapsed_time_ = 0.1f;
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
	camera_->UpdateViewMatrix();
	SetBulletPosition();
	 
	for (const auto& object : deferred_rendering_objects_)
	{
		object->AnimateObject();
	}

	for (const auto& object : forward_rendering_objects_)
	{
		object->AnimateObject();
	}
}

void ObjectsManager::ExecuteCommandList()
{  
	ClearRenderTargetViewAndDepthStencilView();

	ExecuteCommandListPlayer();
	if (test_)
	{
		DeferredRenderingTestExecuteCommandListObjects();
	}
	else
	{
		DeferredRenderingExecuteCommandListObjects();
	} 
	DeferredRenderingExecuteCommandSecondPass();
	ForwardRenderingExecuteCommandListObjects();

	SwapChainManager::GetInstance()->GetDXGISwapChain()->Present(0, 0);
}

DirectX::XMVECTOR ObjectsManager::GetCameraPosition()
{
	return camera_->GetPosition();
}

DirectX::XMVECTOR ObjectsManager::GetCameraLook()
{
	return camera_->GetLook();
}

DirectX::XMMATRIX ObjectsManager::GetCameraView()
{
	return DirectX::XMMATRIX();
}

DirectX::XMMATRIX ObjectsManager::GetCameraProjection()
{
	return DirectX::XMMATRIX();
}

BulletObject* ObjectsManager::GetBullets()
{
	BulletObject* bullet_objects = dynamic_cast<BulletObject*>(forward_rendering_objects_[0].get());
	if (bullet_objects == nullptr)
	{
		throw std::string("BulletObject dynamic_cast Fail");
	}

	return bullet_objects;
}

void ObjectsManager::UpBulletSpeed()
{
	BulletObject* bullet_objects = dynamic_cast<BulletObject*>(forward_rendering_objects_[0].get());
	if (bullet_objects == nullptr)
	{
		throw std::string("BulletObject dynamic_cast Fail");
	}

	bullet_objects->UpBulletSpeed();
}

void ObjectsManager::DownBulletSpeed()
{
	BulletObject* bullet_objects = dynamic_cast<BulletObject*>(forward_rendering_objects_[0].get());
	if (bullet_objects == nullptr)
	{
		throw std::string("BulletObject dynamic_cast Fail");
	}

	bullet_objects->DownBulletSpeed();
}
