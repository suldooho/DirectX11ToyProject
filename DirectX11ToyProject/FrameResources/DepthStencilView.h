#pragma once
#include <wrl.h> 

class DepthStencilView
{
private:
	Microsoft::WRL::ComPtr<class ID3D11Texture2D> d3d11_depth_stencil_buffer_;
	Microsoft::WRL::ComPtr<class ID3D11DepthStencilView> d3d11_depth_stencil_view_;

public:
	void Initialize(unsigned int client_width, unsigned int client_height);
};

