#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "FrameResource.h"

class FrameResourcesManager
{
private:
	static FrameResourcesManager* instance_;

	FrameResourcesManager() = default;
	~FrameResourcesManager() = default;

public:
	FrameResourcesManager(const FrameResourcesManager&) = delete;
	FrameResourcesManager& operator=(const FrameResourcesManager&) = delete;

	static FrameResourcesManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new FrameResourcesManager();
		}

		return instance_;
	}

private:
	std::unordered_map<std::string, std::unique_ptr<FrameResource>> frame_resource_container_;

public:
	void Initialize(unsigned int client_width, unsigned int client_height);

	class FrameResource* GetFrameResource(std::string class_name);
};

