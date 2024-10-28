#pragma once
#include "Mesh.h"
#include <wrl.h> 
#include <string>

class TextureMesh : public Mesh
{
protected:
	Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView> diffuse_;

	Microsoft::WRL::ComPtr<class ID3D11SamplerState> sampler_state_;

protected: 
	class ID3D11ShaderResourceView* LoadTexture(std::string obj_texture_file_path);
	void CreateSamplerState();

public:
	virtual void Initialize(std::string file_path) = 0;
	class ID3D11ShaderResourceView** GetDiffuse();
	class ID3D11SamplerState** GetSampler();
};

