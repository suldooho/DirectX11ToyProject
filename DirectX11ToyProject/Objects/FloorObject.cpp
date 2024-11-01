#include "FloorObject.h"
#include "../framework.h"
#include "../Meshes/FloorMesh.h"
#include "../FrameResources/FloorShader.h"
#include "../FrameResources/OutputMerger.h"

void FloorObject::Initialize()
{
	CreateConstantBuffer();
	SetPosition(0.0f, -10.0f, 0.0f);

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());

	FloorMesh* floor_mesh = dynamic_cast<FloorMesh*>(MeshesManager::GetInstance()->GetMesh("FloorMesh"));
	if (floor_mesh == nullptr)
	{
		throw std::string("FloorMesh dynamic_cast Fail");
	}

	FloorShader* floor_shader = dynamic_cast<FloorShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("FloorShader"));
	if (floor_shader == nullptr)
	{
		throw std::string("FloorShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	deferred_context->IASetPrimitiveTopology(floor_mesh->GetPrimitiveTopology());
	deferred_context->IASetVertexBuffers(0, 1, floor_mesh->GetVertexBuffer(), floor_mesh->GetStride(), floor_mesh->GetOffset());
	deferred_context->IASetInputLayout(floor_shader->GetInputLayout());
	deferred_context->VSSetShader(floor_shader->GetVertexShader(), nullptr, 0);
	deferred_context->HSSetShader(floor_shader->GetHullShader(), nullptr, 0);
	deferred_context->DSSetShader(floor_shader->GetDomainShader(), nullptr, 0);
	deferred_context->PSSetShader(floor_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport());
	deferred_context->RSSetState(floor_mesh->GetRasterizerState());
	ID3D11RenderTargetView* render_target_views[4] =
	{
		output_merger->GetRenderTargetView("GBufferPositionView"),
		output_merger->GetRenderTargetView("GBufferNormalView"),
		output_merger->GetRenderTargetView("GBufferDiffuseView"),
		output_merger->GetRenderTargetView("GBufferViewDirectionView")
	};
	deferred_context->OMSetRenderTargets(4, render_target_views, output_merger->GetDepthStencilView());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kVertexShaderSlotWorldMatrix_, 1, world_matrix_constant_buffer_.GetAddressOf()); 
	deferred_context->DSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->PSSetShaderResources(0, 1, floor_mesh->GetTextureShaderResourceView("DiffuseView"));
	deferred_context->PSSetShaderResources(1, 1, floor_mesh->GetTextureShaderResourceView("NormalView"));
	deferred_context->DSSetShaderResources(2, 1, floor_mesh->GetTextureShaderResourceView("HeightView"));
	deferred_context->PSSetSamplers(0, 1, floor_mesh->GetSampler());
	deferred_context->Draw(floor_mesh->GetNumVertices(), 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());
}
