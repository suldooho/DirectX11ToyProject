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
	std::unique_ptr<Player> player_;
	std::unique_ptr<Camera> camera_;

	std::vector<std::unique_ptr<MovableObject>> movable_objects_;

private:
	void CreateMovableObjects();
	void ExecuteCommandListMovableObjects();

	void ClearDepthStencilView();

public:
	void Initialize(float client_width, float client_height);

	class ID3D11Buffer** GetAddressOfCameraConstantBuffer() const;
	void ExecuteCommandList();

public:
	const unsigned int kVertexShaderSlotWorldMatrix = 0;
	const unsigned int kCameraShaderSlotWorldMatrix = 1;
};

