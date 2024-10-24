#include "OBJMesh.h"
#include "../framework.h"
#include <fstream>
#include <sstream>

void OBJMesh::LoadVertices(std::string obj_file_path)
{
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
            vertices_.push_back(vertex);
        }
    }

    file.close();
}

void OBJMesh::LoadIndices(std::string obj_indices_file_path)
{
    std::ifstream file(obj_indices_file_path);

    if (!file.is_open()) 
    {
        throw std::string("Can Not Load " + obj_indices_file_path); 
    }

    std::string line;
    unsigned int i1, i2, i3;

    while (std::getline(file, line)) {
        if (line.find("Triangle:") != std::string::npos) {
            std::istringstream iss(line);
            std::string dummy;
            iss >> dummy >> i1 >> i2 >> i3; 

            indices_.push_back(i1);
            indices_.push_back(i2);
            indices_.push_back(i3);
        }
    }

    file.close();
}

void OBJMesh::CreateVertices()
{
    num_vertices_ = vertices_.size();
    stride_ = sizeof(BumpMappingVertex);
    offset_ = 0;
}

void OBJMesh::CreateIndices()
{
    num_indices_ = indices_.size();
    start_index_ = 0;
    base_vertex_ = 0;
}

unsigned int OBJMesh::GetVertexBufferByteWidth()
{
	return sizeof(BumpMappingVertex) * vertices_.size();
}

unsigned int OBJMesh::GetIndexBufferByteWidth()
{
	return sizeof(unsigned int) * indices_.size();
}

void* OBJMesh::GetVertexData()
{
	return vertices_.data();
}

void* OBJMesh::GetIndexData()
{
	return indices_.data();
}

void OBJMesh::Initialize(std::string obj_file_path)
{
    wchar_t current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);
     
    std::wstring w_obj_file_path;
    w_obj_file_path.assign(obj_file_path.begin(), obj_file_path.end());

    w_obj_file_path = std::wstring(current_path) + w_obj_file_path;
    obj_file_path.assign(w_obj_file_path.begin(), w_obj_file_path.end());

    LoadVertices(obj_file_path + ".txt");
    LoadIndices(obj_file_path + "_Index.txt");
	Mesh::Initialize(); 
}
