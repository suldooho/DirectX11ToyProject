#include "Shader.h"
#include "../framework.h"

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* input_element_desc, unsigned int num_elements)
{
	Microsoft::WRL::ComPtr<ID3DBlob> code_blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

	HRESULT result = Device::GetInstace()->GetD3D11Device()->CreateInputLayout(input_element_desc, num_elements, code_blob->GetBufferPointer(), code_blob->GetBufferSize(), d3d11_input_layout_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Input Layout");
	}
}

void Shader::CreateVertexShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point)
{
	Microsoft::WRL::ComPtr<ID3DBlob> code_blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

	HRESULT result = D3DCompileFromFile(shader_file, nullptr, nullptr, entry_point, "vs_5_0", 0, 0, code_blob.GetAddressOf(), error_blob.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Compile Vertex Shader");
	}

	HRESULT result = Device::GetInstace()->GetD3D11Device()->CreateVertexShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), nullptr, d3d11_vertex_shader_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Vertex Shader");
	}
}
void Shader::CreatePixelShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point)
{
	Microsoft::WRL::ComPtr<ID3DBlob> code_blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;
	
	HRESULT result = D3DCompileFromFile(shader_file, nullptr, nullptr, entry_point, "ps_5_0", 0, 0, code_blob.GetAddressOf(), error_blob.GetAddressOf());
	
	if (result != S_OK)
	{
		throw std::string("Can Not Create Pixel Shader");
	}
	
	HRESULT result = Device::GetInstace()->GetD3D11Device()->CreatePixelShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), nullptr, d3d11_pixel_shader_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Pixel Shader");
	}
} 
