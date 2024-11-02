#include "MeshesManager.h"
#include "SkySphereMesh.h"
#include "PlayerMesh.h"
#include "FloorMesh.h"

MeshesManager* MeshesManager::instance_ = nullptr;

void MeshesManager::Initialize()
{
	mesh_container_["SkySphereMesh"] = std::make_unique<SkySphereMesh>();
	SkySphereMesh* sky_sphere_mesh = dynamic_cast<SkySphereMesh*>(mesh_container_["SkySphereMesh"].get());
	sky_sphere_mesh->Initialize("/Resources/SkySphere");
	 
	mesh_container_["PlayerMesh"] = std::make_unique<PlayerMesh>();
	PlayerMesh* player_mesh = dynamic_cast<PlayerMesh*>(mesh_container_["PlayerMesh"].get());
	player_mesh->Initialize("/Resources/Gun");

	mesh_container_["FloorMesh"] = std::make_unique<FloorMesh>();
	FloorMesh* floor_mesh = dynamic_cast<FloorMesh*>(mesh_container_["FloorMesh"].get());
	floor_mesh->Initialize("/Resources/Floor");
} 