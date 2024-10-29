#include "MeshesManager.h"
#include "SkySphereMesh.h"
#include "OBJMesh.h"

MeshesManager* MeshesManager::instance_ = nullptr;

void MeshesManager::Initialize()
{
	mesh_container_["SkySphereMesh"] = std::make_unique<SkySphereMesh>();
	SkySphereMesh* sky_sphere_mesh = dynamic_cast<SkySphereMesh*>(mesh_container_["SkySphereMesh"].get());
	sky_sphere_mesh->Initialize("/Resources/SkySphere");
	 
	mesh_container_["GunMesh"] = std::make_unique<OBJMesh>();
	OBJMesh* obj_mesh = dynamic_cast<OBJMesh*>(mesh_container_["GunMesh"].get());
	obj_mesh->Initialize("/Resources/Gun");
}

Mesh* MeshesManager::GetMesh(std::string class_name)
{
	if (mesh_container_.find(class_name) != mesh_container_.end())
	{
		return mesh_container_[class_name].get();
	}

	throw std::string("Mesh Name Error");
}
