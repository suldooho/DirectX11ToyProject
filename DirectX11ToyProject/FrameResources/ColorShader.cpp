#include "ColorShader.h"
#include "../framework.h" 

void ColorShader::Initialize()
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int num_elements = ARRAYSIZE(input_element_desc);

	CreateInputLayout(input_element_desc, num_elements);
	CreateVertexShaderFromFile(L"../HLSL/Color.hlsli" , "VS");
	CreatePixelShaderFromFile(L"../HLSL/Color.hlsli" , "PS");
}
