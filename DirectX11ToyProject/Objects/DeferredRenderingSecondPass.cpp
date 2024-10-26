#include "DeferredRenderingSecondPass.h"
#include "../framework.h"
#include "../FrameResources/FrameResource.h"
#include "../FrameResources/OutputMerger.h"
#include "../Meshes/OBJMesh.h"
#include "../FrameResources/SecondPassShader.h"

void DeferredRenderingSecondPass::Initialize()
{   
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11_deferred_context;
	DeviceManager::GetInstace()->GetD3D11Device()->CreateDeferredContext(0, d3d11_deferred_context.GetAddressOf());

	OBJMesh* obj_mesh = dynamic_cast<OBJMesh*>(MeshesManager::GetInstace()->GetMesh("GunMesh"));
	if (obj_mesh == nullptr)
	{
		throw std::string("OBJMesh dynamic_cast Fail");
	}

	SecondPassShader* second_pass_shader = dynamic_cast<SecondPassShader*>(FrameResourcesManager::GetInstace()->GetFrameResource("SecondPassShader"));
	if (second_pass_shader == nullptr)
	{
		throw std::string("SecondPassShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstace()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	d3d11_deferred_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d11_deferred_context->IASetInputLayout(nullptr); // SV_VertexID¿¡ ÀÇÁ¸
	d3d11_deferred_context->VSSetShader(second_pass_shader->GetVertexShader(), nullptr, 0);
	d3d11_deferred_context->PSSetShader(second_pass_shader->GetPixelShader(), nullptr, 0);
	d3d11_deferred_context->RSSetViewports(1, output_merger->GetViewport()); 
	d3d11_deferred_context->RSSetState(obj_mesh->GetRasterizerState());
	ID3D11ShaderResourceView* d3d11_shader_resource_views[4] =
	{
		output_merger->GetGBufferPositionShaderResourceView(),
		output_merger->GetGBufferNormalShaderResourceView(),
		output_merger->GetGBufferDiffuseShaderResourceView(),
		output_merger->GetGBufferViewDirectionShaderResourceView()
	}; 
	d3d11_deferred_context->PSSetShaderResources(4, 4, d3d11_shader_resource_views);
	d3d11_deferred_context->PSSetSamplers(0, 1, obj_mesh->GetSampler()); 
	d3d11_deferred_context->OMSetDepthStencilState(output_merger->GetDepthStencilState("SecondPass"), 1);
	ID3D11RenderTargetView* d3d11_render_target_view = output_merger->GetRenderTargetView();
	d3d11_deferred_context->OMSetRenderTargets(1, &d3d11_render_target_view, output_merger->GetDepthStencilView()); 
	d3d11_deferred_context->Draw(6, 0);

	d3d11_deferred_context->FinishCommandList(true, d3d11_command_list_.GetAddressOf());
}
