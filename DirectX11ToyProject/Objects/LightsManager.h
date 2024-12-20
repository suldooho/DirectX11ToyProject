#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <unordered_map>

struct LightCount
{
	unsigned int point_light_count;
	unsigned int spot_light_count;
	unsigned int padding1;// 패딩 (4바이트)
	unsigned int padding2;// 패딩 (4바이트)
};

struct PointLight
{
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT3 position;
	float range; 
};

struct SpotLight
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 direction;
	float angle;
	float time;
	DirectX::XMFLOAT3 padding;
}; 

class LightsManager
{
	static LightsManager* instance_;

	LightsManager() = default;
	~LightsManager() = default;

public:
	LightsManager(const LightsManager&) = delete;
	LightsManager& operator=(const LightsManager&) = delete;

	static LightsManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new LightsManager();
		}

		return instance_;
	}

private:   
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11Buffer>> light_buffer_container_; 
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<class ID3D11ShaderResourceView>> shader_resource_view_container_;
	 
public:
	void Initialize();
	void UpdateLightBuffers();

	class ID3D11ShaderResourceView** GetLightShaderResourceView(std::string view_name);
	class ID3D11Buffer** GetLightBuffer(std::string buffer_name);

private:
	const unsigned int kMaxPointLightNum_ = 100;
	const unsigned int kMaxSpotLightNum_ = 100;
};

