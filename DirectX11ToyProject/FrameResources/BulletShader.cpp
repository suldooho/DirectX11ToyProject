#include "BulletShader.h"
#include "../framework.h"

void BulletShader::Initialize()
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{ 
		{ "INSTANCE_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_PREV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 } 
	};
	unsigned int num_elements = ARRAYSIZE(input_element_desc);

	wchar_t current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);
	std::wstring shader_file_path = std::wstring(current_path) + L"/HLSL/Bullet.hlsl";

	CreateInputLayoutAndVertexShaderFromFile(shader_file_path.c_str(), "VS", input_element_desc, num_elements);
	CreateGeometryShaderFromFile(shader_file_path.c_str(), "main");
	CreatePixelShaderFromFile(shader_file_path.c_str(), "PS");
}
