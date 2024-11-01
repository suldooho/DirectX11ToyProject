#pragma once
#include "Mesh.h"
#include <wrl.h> 
#include <string>
#include <unordered_map>

class TextureMesh : public Mesh
{
protected:
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView>> shader_resource_view_container_;

	Microsoft::WRL::ComPtr<class ID3D11SamplerState> sampler_state_;

protected: 
	class ID3D11ShaderResourceView* LoadTexture(std::string obj_texture_file_path);
	virtual void CreateSamplerState();

public:
	virtual void Initialize(std::string file_path) = 0;
	class ID3D11ShaderResourceView** GetTextureShaderResourceView(std::string texture_name);
	class ID3D11SamplerState** GetSampler();
};

