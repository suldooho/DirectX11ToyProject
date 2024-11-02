#pragma once
#include "Mesh.h"
#include <wrl.h> 
#include <string>
#include <unordered_map>

class TextureComponent
{
private:
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView>> shader_resource_view_container_;

	Microsoft::WRL::ComPtr<class ID3D11SamplerState> sampler_state_;
	 
protected:
	void CreateSamplerState();

public:
	void Initialize();
	void LoadTexture(std::string texture_file_path);

	std::string GetAbsolutePathPath(std::string file_path);

public: // has-a
	class ID3D11ShaderResourceView** GetTextureShaderResourceView(std::string texture_name);
	class ID3D11SamplerState** GetSampler();
};

