#pragma once
#include "Mesh.h"
#include "Vertices.h"
#include <vector>
#include <string>

class OBJMesh : public Mesh
{
private:
	std::vector<BumpMappingVertex> vertices_;
	std::vector<unsigned int> indices_;

private:
	void LoadVertices(std::string obj_file_path);
	void LoadIndices(std::string obj_indices_file_path);

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual unsigned int GetIndexBufferByteWidth() override;

	virtual void* GetVertexData() override;
	virtual void* GetIndexData() override;

public: 
	void Initialize(std::string obj_file_path);
};

