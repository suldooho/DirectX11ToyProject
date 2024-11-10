#pragma once
#include <memory>
#include <vector>
#include "Player.h"
#include "Camera.h"
#include "Object.h"
#include "DeferredRenderingSecondPass.h"

class ObjectsManager
{
	static ObjectsManager* instance_;

	ObjectsManager() = default;
	~ObjectsManager() = default;

public:
	ObjectsManager(const ObjectsManager&) = delete;
	ObjectsManager& operator=(const ObjectsManager&) = delete;

	static ObjectsManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new ObjectsManager();
		}

		return instance_;
	}

private:  
	bool test_;

	int client_width_;
	int client_height_;

	bool mouse_click_;
	float click_elapsed_time_;

	unsigned int camera_direction_;
	float camera_yaw_;
	float camera_pitch_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Camera> camera_;

	std::vector<std::unique_ptr<Object>> forward_rendering_objects_;
	std::vector<std::unique_ptr<Object>> deferred_rendering_objects_;
	std::unique_ptr<DeferredRenderingSecondPass> deferred_rendering_second_pass_;

private:
	void CreateObjects();
	void ExecuteCommandListPlayer();
	void DeferredRenderingExecuteCommandListObjects();
	void DeferredRenderingTestExecuteCommandListObjects();
	void ForwardRenderingExecuteCommandListObjects();
	void DeferredRenderingExecuteCommandSecondPass();

	void ClearRenderTargetViewAndDepthStencilView();

	void MoveCamera(float delta_time);
	void RotateCamera(float delta_time);
	void SetPlayerRotationAndPosition(DirectX::FXMMATRIX camera_world_matrix);
	DirectX::XMMATRIX ScreenToWorld();
	void SetBulletPosition();

public:
	void Initialize(float client_width, float client_height);

	void ActiveTest();

	void PushButton(unsigned int direction);
	void ReleaseButton(unsigned int direction);
	void SetRotationValue(float yaw, float pitch);

	void PushMouseLeftButton();

public:
	class ID3D11Buffer** GetCameraConstantBuffer() const;

	void AnimateObjects();
	void ExecuteCommandList();
	DirectX::XMVECTOR GetCameraPosition();
	DirectX::XMVECTOR GetCameraLook();
	DirectX::XMMATRIX GetCameraView();
	DirectX::XMMATRIX GetCameraProjection();

	class BulletObject* GetBullets();

	void UpBulletSpeed();
	void DownBulletSpeed();
public:
	const float kRotationSensitivity_ = 2.0f;

	const unsigned int kVertexShaderSlotWorldMatrix_ = 0;
	const unsigned int kInstanceVertexShaderSlotWorldMatrix_ = 0;
	const unsigned int kCameraShaderSlotWorldMatrix_ = 1;

	const unsigned int kAKey_ = 0x01;
	const unsigned int kSKey_ = 0x02;
	const unsigned int kDKey_ = 0x04;
	const unsigned int kWKey_ = 0x08;
};

