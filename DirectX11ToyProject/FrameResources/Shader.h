#pragma once
#include "FrameResource.h"
#include <wrl.h> 

class Shader : public FrameResource
{
private:
	Microsoft::WRL::ComPtr<class ID3D11InputLayout> d3d11_input_layout_;
	Microsoft::WRL::ComPtr<class ID3D11VertexShader> d3d11_vertex_shader_; 
	Microsoft::WRL::ComPtr<class ID3D11PixelShader> d3d11_pixel_shader_;

protected:
	void CreateInputLayoutAndVertexShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point, class D3D11_INPUT_ELEMENT_DESC* input_element_desc, unsigned int num_elements);
	void CreatePixelShaderFromFile(LPCWSTR shader_file, LPCSTR entry_point);
	  
public:
	virtual void Initialize() = 0;

public:
	class ID3D11InputLayout* GetInputLayout() const;
	class ID3D11VertexShader* GetVertexShader() const;
	class ID3D11PixelShader* GetPixelShader() const;

};

