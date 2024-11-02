#include "SkySphereObject.h"
#include "../framework.h"
#include "../Meshes/SkySphereMesh.h"
#include "../FrameResources/SkySphereShader.h"
#include "../FrameResources/OutputMerger.h"

void SkySphereObject::Initialize()
{
	CreateConstantBuffer(); 

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());

	SkySphereMesh* sky_sphere_mesh = dynamic_cast<SkySphereMesh*>(MeshesManager::GetInstance()->GetMesh("SkySphereMesh"));
	if (sky_sphere_mesh == nullptr)
	{
		throw std::string("SkySphereMesh dynamic_cast Fail");
	}

	SkySphereShader* sky_sphere_shader = dynamic_cast<SkySphereShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("SkySphereShader"));
	if (sky_sphere_shader == nullptr)
	{
		throw std::string("SkySphereShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	deferred_context->IASetPrimitiveTopology(sky_sphere_mesh->GetPrimitiveTopology());
	ID3D11Buffer* buffer_pointers[1] = { sky_sphere_mesh->GetVertexBuffer() };
	deferred_context->IASetVertexBuffers(0, 1, buffer_pointers, sky_sphere_mesh->GetStride(), sky_sphere_mesh->GetOffset());
	deferred_context->IASetIndexBuffer(sky_sphere_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deferred_context->IASetInputLayout(sky_sphere_shader->GetInputLayout());
	deferred_context->VSSetShader(sky_sphere_shader->GetVertexShader(), nullptr, 0);
	deferred_context->PSSetShader(sky_sphere_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport());
	deferred_context->RSSetState(sky_sphere_mesh->GetRasterizerState()); 
	ID3D11RenderTargetView* render_target_views[4] =
	{
		output_merger->GetRenderTargetView("GBufferPositionView"),
		output_merger->GetRenderTargetView("GBufferNormalView"),
		output_merger->GetRenderTargetView("GBufferDiffuseView"),
		output_merger->GetRenderTargetView("GBufferViewDirectionView")
	};
	deferred_context->OMSetRenderTargets(4, render_target_views, output_merger->GetDepthStencilView());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kVertexShaderSlotWorldMatrix_, 1, world_matrix_constant_buffer_.GetAddressOf());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->PSSetShaderResources(0, 1, sky_sphere_mesh->texture_component_->GetTextureShaderResourceView("DiffuseView"));
	deferred_context->PSSetSamplers(0, 1, sky_sphere_mesh->texture_component_->GetSampler());
	deferred_context->DrawIndexed(sky_sphere_mesh->GetNumIndices(), 0, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());

}
