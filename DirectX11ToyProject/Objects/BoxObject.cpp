#include "BoxObject.h"
#include "../framework.h"
#include "../Meshes/MeshesManager.h"
#include "../Meshes/BoxMesh.h"
#include "../FrameResources/FrameResource.h"
#include "../FrameResources/ColorShader.h"
#include "../FrameResources/OutputMerger.h"

void BoxObject::Initialize()
{
	CreateConstantBuffer();

	SetPosition(0.0f, 0.0f, 10.0f);

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());

	BoxMesh* box_mesh = dynamic_cast<BoxMesh*>(MeshesManager::GetInstance()->GetMesh("BoxMesh"));
	if (box_mesh == nullptr)
	{
		throw std::string("BoxMesh dynamic_cast Fail");
	}

	ColorShader* color_shader = dynamic_cast<ColorShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("ColorShader"));
	if (color_shader == nullptr)
	{
		throw std::string("ColorShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}
	deferred_context->IASetPrimitiveTopology(box_mesh->GetPrimitiveTopology()); 
	deferred_context->IASetVertexBuffers(0, 1, box_mesh->GetVertexBuffer(), box_mesh->GetStride(), box_mesh->GetOffset());
	deferred_context->IASetIndexBuffer(box_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deferred_context->IASetInputLayout(color_shader->GetInputLayout());
	deferred_context->VSSetShader(color_shader->GetVertexShader(), nullptr, 0);
	deferred_context->PSSetShader(color_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport());
	deferred_context->RSSetState(box_mesh->GetRasterizerState());
	ID3D11RenderTargetView* back_buffer_view = output_merger->GetRenderTargetView("BackBufferView");
	deferred_context->OMSetRenderTargets(1, &back_buffer_view, output_merger->GetDepthStencilView());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kVertexShaderSlotWorldMatrix_, 1, world_matrix_constant_buffer_.GetAddressOf());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->DrawIndexed(box_mesh->GetNumIndices(), 0, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());
}
