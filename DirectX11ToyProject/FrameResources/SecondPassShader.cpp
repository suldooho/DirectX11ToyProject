#include "SecondPassShader.h"
#include "../framework.h"

void SecondPassShader::Initialize()
{  
	wchar_t current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);
	std::wstring shader_file_path = std::wstring(current_path) + L"/HLSL/SecondPass.hlsl";

	CreateVertexShaderFromFile(shader_file_path.c_str(), "main");
	CreatePixelShaderFromFile(shader_file_path.c_str(), "PS");
}
