#pragma once
#include <fstream>
#include <d3dcommon.h>

class IncludeHandler : public ID3DInclude
{ 
public: 
    STDMETHOD(Open)(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
    {
        char current_path[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, current_path);     
        std::string shader_file_path = std::string(current_path) + "/HLSL/" + pFileName;
        std::ifstream file(shader_file_path, std::ios::binary | std::ios::ate); 
        if (!file.is_open())
        {
            return E_FAIL;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        char* data = new char[size];
        if (!file.read(data, size))
        {
            delete[] data;
            return E_FAIL;
        }

        *ppData = data;
        *pBytes = static_cast<UINT>(size);
        return S_OK;
    }
     
    STDMETHOD(Close)(LPCVOID pData) override
    {
        delete[] reinterpret_cast<const char*>(pData);
        return S_OK;
    }
};