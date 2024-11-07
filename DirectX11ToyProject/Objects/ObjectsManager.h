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
	unsigned int camera_direction_;
	float camera_yaw_;
	float camera_pitch_;
	const float kRotationSensitivity_ = 2.0f;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Camera> camera_;

	std::vector<std::unique_ptr<Object>> forward_rendering_objects_;
	std::vector<std::unique_ptr<Object>> deferred_rendering_objects_;
	std::unique_ptr<DeferredRenderingSecondPass> deferred_rendering_second_pass_;

private:
	void CreateObjects();
	void ExecuteCommandListPlayer();
	void DeferredRenderingExecuteCommandListObjects();
	void ForwardRenderingExecuteCommandListObjects();
	void DeferredRenderingExecuteCommandSecondPass();

	void ClearRenderTargetViewAndDepthStencilView();

	void MoveCamera(float delta_time);
	void RotateCamera(float delta_time);
	void SetPlayerRotationAndPosition(DirectX::FXMMATRIX camera_world_matrix);

public:
	void Initialize(float client_width, float client_height);

	void PushButton(unsigned int direction);
	void ReleaseButton(unsigned int direction);
	void SetRotationValue(float yaw, float pitch);

public:
	class ID3D11Buffer** GetCameraConstantBuffer() const;

	void AnimateObjects();
	void ExecuteCommandList();
	DirectX::XMVECTOR GetCameraPosition();

public:
	const unsigned int kVertexShaderSlotWorldMatrix_ = 0;
	const unsigned int kInstanceVertexShaderSlotWorldMatrix_ = 0;
	const unsigned int kCameraShaderSlotWorldMatrix_ = 1;

	const unsigned int kAKey_ = 0x01;
	const unsigned int kSKey_ = 0x02;
	const unsigned int kDKey_ = 0x04;
	const unsigned int kWKey_ = 0x08;
};

