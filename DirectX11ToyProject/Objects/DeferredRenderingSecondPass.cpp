#include "DeferredRenderingSecondPass.h"
#include "../framework.h"
#include "../FrameResources/FrameResource.h"
#include "../FrameResources/OutputMerger.h"
#include "../Meshes/PlayerMesh.h"
#include "../FrameResources/SecondPassShader.h"

void DeferredRenderingSecondPass::Initialize()
{   
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());

	PlayerMesh* player_mesh = dynamic_cast<PlayerMesh*>(MeshesManager::GetInstance()->GetMesh<BumpMappingVertex>("PlayerMesh"));
	if (player_mesh == nullptr)
	{
		throw std::string("PlayerMesh dynamic_cast Fail");
	}

	SecondPassShader* second_pass_shader = dynamic_cast<SecondPassShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("SecondPassShader"));
	if (second_pass_shader == nullptr)
	{
		throw std::string("SecondPassShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	deferred_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deferred_context->IASetInputLayout(nullptr); // SV_VertexID�� ����
	deferred_context->VSSetShader(second_pass_shader->GetVertexShader(), nullptr, 0);
	deferred_context->PSSetShader(second_pass_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport()); 
	deferred_context->RSSetState(player_mesh->GetRasterizerState());
	ID3D11ShaderResourceView* shader_resource_views[4] =
	{
		output_merger->GetShaderResourceView("GBufferPositionView"),
		output_merger->GetShaderResourceView("GBufferNormalView"),
		output_merger->GetShaderResourceView("GBufferDiffuseView"),
		output_merger->GetShaderResourceView("GBufferViewDirectionView")
	}; 
	deferred_context->PSSetShaderResources(4, 4, shader_resource_views);
	deferred_context->PSSetShaderResources(0, 1, LightsManager::GetInstance()->GetLightShaderResourceView("PointLightView"));
	deferred_context->PSSetShaderResources(1, 1, LightsManager::GetInstance()->GetLightShaderResourceView("SpotLightView"));
	deferred_context->PSSetConstantBuffers(2, 1, LightsManager::GetInstance()->GetLightBuffer("LightCountBuffer"));
	deferred_context->PSSetSamplers(0, 1, player_mesh->texture_component_->GetSampler());
	deferred_context->OMSetDepthStencilState(output_merger->GetDepthStencilState("SecondPassState"), 0);
	ID3D11RenderTargetView* back_buffer_view = output_merger->GetRenderTargetView("BackBufferView");
	deferred_context->OMSetRenderTargets(1, &back_buffer_view, output_merger->GetDepthStencilView());
	deferred_context->Draw(6, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());
}
