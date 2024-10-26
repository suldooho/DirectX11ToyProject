#include "BoxObject.h"
#include "../framework.h"
#include "../Meshes/MeshesManager.h"
#include "../Meshes/BoxMesh.h"
#include "../FrameResources/FrameResource.h"
#include "../FrameResources/ColorShader.h"
#include "../FrameResources/OutputMerger.h"

void BoxObject::Initialize()
{
	MovableObject::Initialize();

	SetPosition(0.0f, 0.0f, 10.0f);

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11_deferred_context;
	DeviceManager::GetInstace()->GetD3D11Device()->CreateDeferredContext(0, d3d11_deferred_context.GetAddressOf());

	BoxMesh* box_mesh = dynamic_cast<BoxMesh*>(MeshesManager::GetInstace()->GetMesh("BoxMesh"));
	if (box_mesh == nullptr)
	{
		throw std::string("BoxMesh dynamic_cast Fail");
	}

	ColorShader* color_shader = dynamic_cast<ColorShader*>(FrameResourcesManager::GetInstace()->GetFrameResource("ColorShader"));
	if (color_shader == nullptr)
	{
		throw std::string("ColorShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstace()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}
	d3d11_deferred_context->IASetPrimitiveTopology(box_mesh->GetPrimitiveTopology()); 
	d3d11_deferred_context->IASetVertexBuffers(0, 1, box_mesh->GetVertexBuffer(), box_mesh->GetStride(), box_mesh->GetOffset());
	d3d11_deferred_context->IASetIndexBuffer(box_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	d3d11_deferred_context->IASetInputLayout(color_shader->GetInputLayout());
	d3d11_deferred_context->VSSetShader(color_shader->GetVertexShader(), nullptr, 0);
	d3d11_deferred_context->PSSetShader(color_shader->GetPixelShader(), nullptr, 0);
	d3d11_deferred_context->RSSetViewports(1, output_merger->GetViewport());
	d3d11_deferred_context->RSSetState(box_mesh->GetRasterizerState());
	ID3D11RenderTargetView* d3d11_render_target_view = output_merger->GetRenderTargetView();
	d3d11_deferred_context->OMSetRenderTargets(1, &d3d11_render_target_view, output_merger->GetDepthStencilView());//
	d3d11_deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstace()->kVertexShaderSlotWorldMatrix_, 1, d3d11_world_matrix_constant_buffer_.GetAddressOf());
	d3d11_deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstace()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstace()->GetAddressOfCameraConstantBuffer());
	d3d11_deferred_context->DrawIndexed(box_mesh->GetNumIndices(), 0, 0);

	d3d11_deferred_context->FinishCommandList(true, d3d11_command_list_.GetAddressOf());
}
