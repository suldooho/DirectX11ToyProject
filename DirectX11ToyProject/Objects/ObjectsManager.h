#pragma once
#include <memory>
#include <vector>
#include "Player.h"
#include "Camera.h"
#include "MovableObject.h"

class ObjectsManager
{
	static ObjectsManager* instance_;

	ObjectsManager() = default;
	~ObjectsManager() = default;

public:
	ObjectsManager(const ObjectsManager&) = delete;
	ObjectsManager& operator=(const ObjectsManager&) = delete;

	static ObjectsManager* GetInstace()
	{
		if (instance_ == nullptr)
		{
			instance_ = new ObjectsManager();
		}

		return instance_;
	}

private:
	unsigned int direction_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Camera> camera_;

	std::vector<std::unique_ptr<MovableObject>> movable_objects_;

private:
	void CreateMovableObjects();
	void ExecuteCommandListMovableObjects();

	void ClearDepthStencilView();

	void PlayerMove(float delta_time);

public:
	void Initialize(float client_width, float client_height);

	void PushButton(unsigned int direction);
	void ReleaseButton(unsigned int direction);

public:
	class ID3D11Buffer** GetAddressOfCameraConstantBuffer() const;

	void AnimateObjects();
	void ExecuteCommandList();

public:
	const unsigned int kVertexShaderSlotWorldMatrix = 0;
	const unsigned int kCameraShaderSlotWorldMatrix = 1;

	const unsigned int kAKey = 0x01;
	const unsigned int kSKey = 0x02;
	const unsigned int kDKey = 0x04;
	const unsigned int kWKey = 0x08;
};

