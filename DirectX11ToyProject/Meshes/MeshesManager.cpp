#include "MeshesManager.h"
#include "BoxMesh.h"

MeshesManager* MeshesManager::instance_ = nullptr;

void MeshesManager::Initialize()
{
	mesh_container_["BoxMesh"] = std::make_unique<BoxMesh>();
	BoxMesh* box_mesh = dynamic_cast<BoxMesh*>(mesh_container_["BoxMesh"].get());
	box_mesh->Initialize();
}

Mesh* MeshesManager::GetMesh(std::string class_name)
{
	if (mesh_container_.find(class_name) != mesh_container_.end())
	{
		return mesh_container_[class_name].get();
	}

	throw std::string("Mesh Name Error");
}
