#include "FrameResourcesManager.h"
#include "../framework.h"
#include "ColorShader.h"
#include "OutputMerger.h"

FrameResourcesManager* FrameResourcesManager::instance_ = nullptr;

void FrameResourcesManager::Initialize(unsigned int client_width, unsigned int client_height)
{
	frame_resource_container_["ColorShader"] = std::make_unique<ColorShader>();
	ColorShader* color_shader = dynamic_cast<ColorShader*>(frame_resource_container_["ColorShader"].get());
	color_shader->Initialize();

	frame_resource_container_["OutputMerger"] = std::make_unique<OutputMerger>();
	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(frame_resource_container_["OutputMerger"].get());
	output_merger->Initialize(client_width, client_height);
}

FrameResource* FrameResourcesManager::GetFrameResource(std::string class_name)
{ 
	if (frame_resource_container_.find(class_name) != frame_resource_container_.end())
	{
		return frame_resource_container_[class_name].get();
	}

	throw std::string("Frame Resource Name Error");
}  