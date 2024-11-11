#include "TestScreenShader.h"
#include "../framework.h"

void TestScreenShader::Initialize()
{
	wchar_t current_path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_path);
	std::wstring shader_file_path = std::wstring(current_path) + L"/HLSL/TestScreen.hlsl";

	CreateVertexShaderFromFile(shader_file_path.c_str(), "main");
	CreatePixelShaderFromFile(shader_file_path.c_str(), "PS");
}
