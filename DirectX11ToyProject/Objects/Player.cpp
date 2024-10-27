#include "Player.h"
#include "../framework.h"
#include "../Meshes/MeshesManager.h"
#include "../Meshes/BoxMesh.h"
#include "../Meshes/OBJMesh.h"
#include "../FrameResources/FrameResource.h"
#include "../FrameResources/BumpMappingShader.h"
#include "../FrameResources/OutputMerger.h"
 
void Player::Initialize()
{
	MovableObject::Initialize();
	SetPosition(kPlayerToCameraOffset.x, kPlayerToCameraOffset.y, kPlayerToCameraOffset.z); 

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11_deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, d3d11_deferred_context.GetAddressOf());
	 
	OBJMesh* obj_mesh = dynamic_cast<OBJMesh*>(MeshesManager::GetInstance()->GetMesh("GunMesh"));
	if (obj_mesh == nullptr)
	{
		throw std::string("OBJMesh dynamic_cast Fail");
	}

	BumpMappingShader* bump_mapping_shader = dynamic_cast<BumpMappingShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("BumpMappingShader"));
	if (bump_mapping_shader == nullptr)
	{
		throw std::string("BumpMappingShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	} 

	d3d11_deferred_context->IASetPrimitiveTopology(obj_mesh->GetPrimitiveTopology());  
	d3d11_deferred_context->IASetVertexBuffers(0, 1, obj_mesh->GetVertexBuffer(), obj_mesh->GetStride(), obj_mesh->GetOffset());
	d3d11_deferred_context->IASetIndexBuffer(obj_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	d3d11_deferred_context->IASetInputLayout(bump_mapping_shader->GetInputLayout());
	d3d11_deferred_context->VSSetShader(bump_mapping_shader->GetVertexShader(), nullptr, 0);
	d3d11_deferred_context->PSSetShader(bump_mapping_shader->GetPixelShader(), nullptr, 0);
	d3d11_deferred_context->RSSetViewports(1, output_merger->GetViewport());
	d3d11_deferred_context->RSSetState(obj_mesh->GetRasterizerState());
	ID3D11RenderTargetView* d3d11_render_target_views[4] = 
	{ 
		output_merger->GetGBufferPositionRenderTargetView(), 
		output_merger->GetGBufferNormalRenderTargetView(), 
		output_merger->GetGBufferDiffuseRenderTargetView(), 
		output_merger->GetGBufferViewDirectionRenderTargetView() 
	};
	d3d11_deferred_context->OMSetRenderTargets(4, d3d11_render_target_views, output_merger->GetDepthStencilView());
	d3d11_deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kVertexShaderSlotWorldMatrix_, 1, d3d11_world_matrix_constant_buffer_.GetAddressOf());
	d3d11_deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetAddressOfCameraConstantBuffer());
	d3d11_deferred_context->PSSetShaderResources(0, 1, obj_mesh->GetDiffuse());
	d3d11_deferred_context->PSSetShaderResources(1, 1, obj_mesh->GetNormal()); 
	d3d11_deferred_context->PSSetSamplers(0, 1, obj_mesh->GetSampler());
	d3d11_deferred_context->DrawIndexed(obj_mesh->GetNumIndices(), 0, 0);

	d3d11_deferred_context->FinishCommandList(true, d3d11_command_list_.GetAddressOf()); 
}

void Player::SetRotationAndPosition(DirectX::FXMMATRIX camera_world_matrix)
{ 
	DirectX::XMStoreFloat4x4A(&world_matrix_, camera_world_matrix); 
	DirectX::XMVECTOR offset_vector = DirectX::XMLoadFloat3(&kPlayerToCameraOffset);
	offset_vector = DirectX::XMVector3Transform(offset_vector, camera_world_matrix);  // offset_vector is x, y, z, 1.0f ... !?!?

	world_matrix_._41 = DirectX::XMVectorGetX(offset_vector);
	world_matrix_._42 = DirectX::XMVectorGetY(offset_vector);
	world_matrix_._43 = DirectX::XMVectorGetZ(offset_vector);
}
