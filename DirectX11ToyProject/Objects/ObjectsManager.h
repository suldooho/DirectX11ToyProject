#pragma once
#include <memory>
#include "Player.h"
#include "Camera.h"

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

public:
	void Initialize(float client_width, float client_height);

	class ID3D11Buffer** GetAddressOfCameraConstantBuffer() const;
	void ExecuteCommandList();

public:
	const unsigned int kVertexShaderSlotWorldMatrix = 0;
	const unsigned int kCameraShaderSlotWorldMatrix = 1;
};

