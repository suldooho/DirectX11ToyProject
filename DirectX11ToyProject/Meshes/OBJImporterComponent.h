#pragma once 
#include "Vertices.h"
#include <vector>
#include <string> 
#include <memory>

class OBJImporterComponent 
{  
public:
	std::string GetAbsolutePathPath(std::string file_path);

	std::unique_ptr<std::vector<BumpMappingVertex>> LoadVertices(std::string obj_file_path);
	std::unique_ptr<std::vector<unsigned int>> LoadIndices(std::string obj_file_path);
};

