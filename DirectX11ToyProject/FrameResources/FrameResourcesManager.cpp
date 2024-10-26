#include "FrameResourcesManager.h"
#include "../framework.h"
#include "ColorShader.h"
#include "BumpMappingShader.h"
#include "OutputMerger.h"
#include "SecondPassShader.h"

FrameResourcesManager* FrameResourcesManager::instance_ = nullptr;

void FrameResourcesManager::Initialize(unsigned int client_width, unsigned int client_height)
{
	frame_resource_container_["ColorShader"] = std::make_unique<ColorShader>();
	ColorShader* color_shader = dynamic_cast<ColorShader*>(frame_resource_container_["ColorShader"].get());
	color_shader->Initialize();

	frame_resource_container_["BumpMappingShader"] = std::make_unique<BumpMappingShader>();
	BumpMappingShader* bump_mapping_shader = dynamic_cast<BumpMappingShader*>(frame_resource_container_["BumpMappingShader"].get());
	bump_mapping_shader->Initialize();

	frame_resource_container_["OutputMerger"] = std::make_unique<OutputMerger>();
	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(frame_resource_container_["OutputMerger"].get());
	output_merger->Initialize(client_width, client_height);

	frame_resource_container_["SecondPassShader"] = std::make_unique<SecondPassShader>();
	SecondPassShader* second_pass_shader = dynamic_cast<SecondPassShader*>(frame_resource_container_["SecondPassShader"].get());
	second_pass_shader->Initialize();
}

FrameResource* FrameResourcesManager::GetFrameResource(std::string class_name)
{ 
	if (frame_resource_container_.find(class_name) != frame_resource_container_.end())
	{
		return frame_resource_container_[class_name].get();
	}

	throw std::string("Frame Resource Name Error");
}  