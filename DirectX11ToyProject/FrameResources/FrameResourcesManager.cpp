#include "FrameResourcesManager.h"
#include "../framework.h"
#include "SkySphereShader.h"
#include "BumpMappingShader.h"
#include "FloorShader.h"
#include "EnemyShader.h"
#include "BulletShader.h"
#include "OutputMerger.h"
#include "SecondPassShader.h"
#include "TestScreenShader.h"

FrameResourcesManager* FrameResourcesManager::instance_ = nullptr;

void FrameResourcesManager::Initialize(unsigned int client_width, unsigned int client_height)
{
	frame_resource_container_["SkySphereShader"] = std::make_unique<SkySphereShader>();
	SkySphereShader* sky_sphere_shader = dynamic_cast<SkySphereShader*>(frame_resource_container_["SkySphereShader"].get());
	sky_sphere_shader->Initialize();

	frame_resource_container_["BumpMappingShader"] = std::make_unique<BumpMappingShader>();
	BumpMappingShader* bump_mapping_shader = dynamic_cast<BumpMappingShader*>(frame_resource_container_["BumpMappingShader"].get());
	bump_mapping_shader->Initialize();

	frame_resource_container_["FloorShader"] = std::make_unique<FloorShader>();
	FloorShader* floor_shader = dynamic_cast<FloorShader*>(frame_resource_container_["FloorShader"].get());
	floor_shader->Initialize();

	frame_resource_container_["EnemyShader"] = std::make_unique<EnemyShader>();
	EnemyShader* enemy_shader = dynamic_cast<EnemyShader*>(frame_resource_container_["EnemyShader"].get());
	enemy_shader->Initialize();

	frame_resource_container_["BulletShader"] = std::make_unique<BulletShader>();
	BulletShader* bullet_shader = dynamic_cast<BulletShader*>(frame_resource_container_["BulletShader"].get());
	bullet_shader->Initialize();

	frame_resource_container_["OutputMerger"] = std::make_unique<OutputMerger>();
	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(frame_resource_container_["OutputMerger"].get());
	output_merger->Initialize(client_width, client_height);

	frame_resource_container_["SecondPassShader"] = std::make_unique<SecondPassShader>();
	SecondPassShader* second_pass_shader = dynamic_cast<SecondPassShader*>(frame_resource_container_["SecondPassShader"].get());
	second_pass_shader->Initialize();

	frame_resource_container_["TestScreenShader"] = std::make_unique<TestScreenShader>();
	TestScreenShader* test_screen_shader = dynamic_cast<TestScreenShader*>(frame_resource_container_["TestScreenShader"].get());
	test_screen_shader->Initialize();
}

FrameResource* FrameResourcesManager::GetFrameResource(std::string class_name)
{ 
	if (frame_resource_container_.find(class_name) != frame_resource_container_.end())
	{
		return frame_resource_container_[class_name].get();
	}

	throw std::string("Frame Resource Name Error");
}  