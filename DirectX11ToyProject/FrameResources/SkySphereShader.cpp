#include "SkySphereShader.h"
#include "../framework.h"

void SkySphereShader::Initialize()
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } 
	};
	unsigned int num_elements = ARRAYSIZE(input_element_desc);

	wchar_t current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);
	std::wstring shader_file_path = std::wstring(current_path) + L"/HLSL/SkySphere.hlsl";

	CreateInputLayoutAndVertexShaderFromFile(shader_file_path.c_str(), "main", input_element_desc, num_elements);
	CreatePixelShaderFromFile(shader_file_path.c_str(), "PS");
}
