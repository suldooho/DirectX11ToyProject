#pragma once
#include "Mesh.h"
#include "Vertices.h" 
#include "TextureComponent.h"
#include <vector>
#include <string>
#include <memory>

class FloorMesh : public Mesh<TextureVertex>
{ 
private: 
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> test_rasterizer_state_;

public: // Component´Â public
	std::unique_ptr<TextureComponent> texture_component_;

protected:
	virtual void CreateIndexBuffer() override; 
	virtual void CreateRasterizerState() override; 
	virtual void CreateVertices() override;
	virtual void CreateIndices() override; 

	void CreateTestRasterizerState();

public:
	virtual void Initialize(const std::string& file_name) override;

	ID3D11RasterizerState* GetTestRasterizerState();
};


	 