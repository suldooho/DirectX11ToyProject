#pragma once
#include "TextureMesh.h"
#include "Vertices.h"
#include <vector>
#include <string>
#include <wrl.h>

class OBJMesh : public TextureMesh
{
private:
	std::vector<BumpMappingVertex> vertices_; 

	Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView> normal_; 

private:
	void LoadVertices(std::string obj_file_path);
	void LoadIndices(std::string obj_indices_file_path);

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;  
	virtual void* GetVertexData() override;  

public: 
	void Initialize(std::string obj_file_path);

	class ID3D11ShaderResourceView** GetNormal();
};

