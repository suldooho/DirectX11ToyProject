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
	std::unordered_map<std::string, std::unique_ptr<MeshBase>> mesh_container_;

public:
	void Initialize();

	template<typename T>
	Mesh<T>* GetMesh(const std::string& class_name);
};

template<typename T>
inline Mesh<T>* MeshesManager::GetMesh(const std::string& class_name)
{
	auto it = mesh_container_.find(class_name);
	if (it != mesh_container_.end())
	{
		Mesh<T>* mesh = dynamic_cast<Mesh<T>*>(it->second.get());

		if (mesh) 
		{
			return mesh;
		}

		throw std::string("Mesh Type Dismatch");
	}

	throw std::string("Mesh Name Error");
}
