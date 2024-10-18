#include "Shader.h"
#include "../framework.h"
#include "IncludeHandler.h"
  
void Shader::CreateInputLayoutAndVertexShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point, D3D11_INPUT_ELEMENT_DESC* input_element_desc, unsigned int num_elements)
{
	IncludeHandler include_handler;
	Microsoft::WRL::ComPtr<ID3DBlob> code_blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

	HRESULT result = D3DCompileFromFile(shader_file, nullptr, &include_handler, entry_point, "vs_5_0", 0, 0, code_blob.GetAddressOf(), error_blob.GetAddressOf());
	
	if (result != S_OK)
	{
		OutputDebugStringA((char*)error_blob->GetBufferPointer());
		throw std::string("Can Not Compile Vertex Shader");
	}

	result = DeviceManager::GetInstace()->GetD3D11Device()->CreateInputLayout(input_element_desc, num_elements, code_blob->GetBufferPointer(), code_blob->GetBufferSize(), d3d11_input_layout_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Input Layout");
	}

	result = DeviceManager::GetInstace()->GetD3D11Device()->CreateVertexShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), nullptr, d3d11_vertex_shader_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Vertex Shader");
	}
}

void Shader::CreatePixelShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point)
{
	IncludeHandler include_handler;
	Microsoft::WRL::ComPtr<ID3DBlob> code_blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;
	
	HRESULT result = D3DCompileFromFile(shader_file, nullptr, &include_handler, entry_point, "ps_5_0", 0, 0, code_blob.GetAddressOf(), error_blob.GetAddressOf());
	
	if (result != S_OK)
	{
		throw std::string("Can Not Compile Pixel Shader");
	}
	
	result = DeviceManager::GetInstace()->GetD3D11Device()->CreatePixelShader(code_blob->GetBufferPointer(), code_blob->GetBufferSize(), nullptr, d3d11_pixel_shader_.GetAddressOf());

	if (result != S_OK)
	{
		throw std::string("Can Not Create Pixel Shader");
	}
}

ID3D11InputLayout* Shader::GetInputLayout() const
{
	return d3d11_input_layout_.Get();
}

ID3D11VertexShader* Shader::GetVertexShader() const
{
	return d3d11_vertex_shader_.Get();
}

ID3D11PixelShader* Shader::GetPixelShader() const
{
	return d3d11_pixel_shader_.Get();
}

