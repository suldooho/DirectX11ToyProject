#pragma once
#include "FrameResource.h"
#include <wrl.h> 

class Shader : public FrameResource
{
private:
	Microsoft::WRL::ComPtr<class ID3D11InputLayout> input_layout_;
	Microsoft::WRL::ComPtr<class ID3D11VertexShader> vertex_shader_;
	Microsoft::WRL::ComPtr<class ID3D11HullShader> hull_shader_;
	Microsoft::WRL::ComPtr<class ID3D11DomainShader> domain_shader_;
	Microsoft::WRL::ComPtr<class ID3D11GeometryShader> geometry_shader_;
	Microsoft::WRL::ComPtr<class ID3D11PixelShader> pixel_shader_; 

protected:
	void CreateVertexShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point);
	void CreateInputLayoutAndVertexShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point, class D3D11_INPUT_ELEMENT_DESC* input_element_desc, unsigned int num_elements);
	void CreateHullShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point);
	void CreateDomainShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point);
	void CreateGeometryShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point);
	void CreatePixelShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point); 

public:
	virtual void Initialize() = 0;

public:
	class ID3D11InputLayout* GetInputLayout() const;
	class ID3D11VertexShader* GetVertexShader() const;
	class ID3D11HullShader* GetHullShader() const;
	class ID3D11DomainShader* GetDomainShader() const;
	class ID3D11GeometryShader* GetGeometryShader() const;
	class ID3D11PixelShader* GetPixelShader() const; 
};

