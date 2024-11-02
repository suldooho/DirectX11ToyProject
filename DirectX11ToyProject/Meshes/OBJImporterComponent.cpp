#include "OBJImporterComponent.h"
#include "../framework.h"
#include <fstream>
#include <sstream>
 
void OBJImporterComponent::SetAbsolutePathPath()
{
    wchar_t current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);
    std::wstring w_obj_file_path;
    w_obj_file_path.assign(file_name_.begin(), file_name_.end());
    w_obj_file_path = std::wstring(current_path) + w_obj_file_path;
    file_name_.assign(w_obj_file_path.begin(), w_obj_file_path.end()); 
}

std::string OBJImporterComponent::GetAbsolutePathPath()
{
    return file_name_;
}

std::unique_ptr<std::vector<BumpMappingVertex>> OBJImporterComponent::LoadVertices(std::string obj_file_path)
{
    auto vertices_ = std::make_unique<std::vector<BumpMappingVertex>>(); // heap memory~!

    std::ifstream file(obj_file_path);

    if (!file.is_open()) 
    {
        throw std::string("Can Not Load " + obj_file_path);
    }

    std::string line;
    BumpMappingVertex vertex;

    while (std::getline(file, line)) {
        std::istringstream iss(line);

        if (line.find("Position:") != std::string::npos) {
            iss.ignore(10); // "Position "
            iss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
        } 
        else if (line.find("TexCoord:") != std::string::npos) {
            iss.ignore(10); // "TexCoord: " 
            iss >> vertex.texcoord.x >> vertex.texcoord.y;
        } 
        else if (line.find("Tangent:") != std::string::npos) {
            iss.ignore(9); // "Tangent: " 
            iss >> vertex.tangent.x >> vertex.tangent.y >> vertex.tangent.z;
        } 
        else if (line.find("Bitangent:") != std::string::npos) {
            iss.ignore(11); // "Bitangent: "
            iss >> vertex.bitangent.x >> vertex.bitangent.y >> vertex.bitangent.z;
        } 
        else if (line.find("Normal:") != std::string::npos) {
            iss.ignore(8); // "Normal: " 
            iss >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;
        }
        else if (line.find("-------------------------") != std::string::npos) {
            vertices_->emplace_back(vertex);
        }
    }

    file.close();

    return vertices_;
} 

std::unique_ptr<std::vector<unsigned int>> OBJImporterComponent::LoadIndices(std::string obj_file_path)
{
    auto indices_ = std::make_unique<std::vector<unsigned int>>(); // heap memory~!

    std::ifstream file(obj_file_path);

    if (!file.is_open()) 
    {
        throw std::string("Can Not Load " + obj_file_path);
    }

    std::string line;
    unsigned int i1, i2, i3;
     
    while (std::getline(file, line)) 
    {
        if (line.find("Triangle:") != std::string::npos) 
        {
            std::istringstream iss(line);
            std::string dummy;
            iss >> dummy >> i1 >> i2 >> i3;

            indices_->emplace_back(i1);
            indices_->emplace_back(i2);
            indices_->emplace_back(i3);
        }
    }

    file.close();

    return indices_;
}
void OBJImporterComponent::Initialize(std::string file_name)
{
    file_name_ = file_name;
    SetAbsolutePathPath();
}
