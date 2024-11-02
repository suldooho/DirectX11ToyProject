#pragma once 
#include "Vertices.h"
#include <vector>
#include <string> 
#include <memory>

class OBJImporterComponent 
{    
private:
	std::string file_name_;

	void SetAbsolutePathPath();

public:
	std::string GetAbsolutePathPath();

	std::unique_ptr<std::vector<BumpMappingVertex>> LoadVertices(std::string obj_file_path);
	std::unique_ptr<std::vector<unsigned int>> LoadIndices(std::string obj_file_path);

	void Initialize(std::string file_name);
};

