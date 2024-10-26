#pragma once
#include "Mesh.h"
#include "Vertices.h"
#include <vector>
#include <string>
#include <wrl.h>

class OBJMesh : public Mesh
{
private:
	std::vector<BumpMappingVertex> vertices_;
	std::vector<unsigned int> indices_;

	Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView> diffuse_;
	Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView> normal_; 
	Microsoft::WRL::ComPtr<class ID3D11SamplerState> sampler_state_;

private:
	void LoadVertices(std::string obj_file_path);
	void LoadIndices(std::string obj_indices_file_path);
	class ID3D11ShaderResourceView* LoadTexture(std::string obj_texture_file_path);
	void CreateSamplerState();

protected:
	virtual void CreateVertices() override;
	virtual void CreateIndices() override;

	virtual unsigned int GetVertexBufferByteWidth() override;
	virtual unsigned int GetIndexBufferByteWidth() override;

	virtual void* GetVertexData() override;
	virtual void* GetIndexData() override;


public: 
	void Initialize(std::string obj_file_path);

	class ID3D11ShaderResourceView** GetDiffuse();
	class ID3D11ShaderResourceView** GetNormal(); 
	class ID3D11SamplerState** GetSampler();
};

