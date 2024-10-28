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

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());
	 
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

	deferred_context->IASetPrimitiveTopology(obj_mesh->GetPrimitiveTopology());  
	deferred_context->IASetVertexBuffers(0, 1, obj_mesh->GetVertexBuffer(), obj_mesh->GetStride(), obj_mesh->GetOffset());
	deferred_context->IASetIndexBuffer(obj_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deferred_context->IASetInputLayout(bump_mapping_shader->GetInputLayout());
	deferred_context->VSSetShader(bump_mapping_shader->GetVertexShader(), nullptr, 0);
	deferred_context->PSSetShader(bump_mapping_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport());
	deferred_context->RSSetState(obj_mesh->GetRasterizerState());
	ID3D11RenderTargetView* render_target_views[4] = 
	{ 
		output_merger->GetRenderTargetView("GBufferPositionView"),
		output_merger->GetRenderTargetView("GBufferNormalView"),
		output_merger->GetRenderTargetView("GBufferDiffuseView"),
		output_merger->GetRenderTargetView("GBufferViewDirectionView")
	};
	deferred_context->OMSetRenderTargets(4, render_target_views, output_merger->GetDepthStencilView());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kVertexShaderSlotWorldMatrix_, 1, world_matrix_constant_buffer_.GetAddressOf());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetAddressOfCameraConstantBuffer());
	deferred_context->PSSetShaderResources(0, 1, obj_mesh->GetDiffuse());
	deferred_context->PSSetShaderResources(1, 1, obj_mesh->GetNormal()); 
	deferred_context->PSSetSamplers(0, 1, obj_mesh->GetSampler());
	deferred_context->DrawIndexed(obj_mesh->GetNumIndices(), 0, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf()); 
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
