#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Mesh.h"

class MeshesManager
{
private:
	static MeshesManager* instance_;

	MeshesManager() = default;
	~MeshesManager() = default;

public:
	MeshesManager(const MeshesManager&) = delete;
	MeshesManager& operator=(const MeshesManager&) = delete;

	static MeshesManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new MeshesManager();
		}

		return instance_;
	}

private:
	std::unordered_map<std::string, std::unique_ptr<Mesh>> mesh_container_;

public:
	void Initialize();
	
	class Mesh* GetMesh(std::string class_name);
};

