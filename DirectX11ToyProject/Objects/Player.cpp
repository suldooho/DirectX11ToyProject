#include "Player.h"
#include "../framework.h"
#include "../Meshes/MeshesManager.h"
#include "../Meshes/BoxMesh.h"
#include "../Meshes/PlayerMesh.h"
#include "../FrameResources/FrameResource.h"
#include "../FrameResources/BumpMappingShader.h"
#include "../FrameResources/OutputMerger.h"
 
void Player::Initialize()
{
	CreateBuffer();
	SetPosition(kPlayerToCameraOffset.x, kPlayerToCameraOffset.y, kPlayerToCameraOffset.z); 

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());
	 
	PlayerMesh* player_mesh = dynamic_cast<PlayerMesh*>(MeshesManager::GetInstance()->GetMesh<BumpMappingVertex>("PlayerMesh"));
	if (player_mesh == nullptr)
	{
		throw std::string("PlayerMesh dynamic_cast Fail");
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

	deferred_context->IASetPrimitiveTopology(player_mesh->GetPrimitiveTopology());
	ID3D11Buffer* buffer_pointers[1] = { player_mesh->GetVertexBuffer() };
	UINT strides[1] = { player_mesh->GetStride() };
	UINT offsets[1] = { player_mesh->GetOffset() };
	deferred_context->IASetVertexBuffers(0, 1, buffer_pointers, strides, offsets);
	deferred_context->IASetIndexBuffer(player_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deferred_context->IASetInputLayout(bump_mapping_shader->GetInputLayout());
	deferred_context->VSSetShader(bump_mapping_shader->GetVertexShader(), nullptr, 0);
	deferred_context->PSSetShader(bump_mapping_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport());
	deferred_context->RSSetState(player_mesh->GetRasterizerState());
	ID3D11RenderTargetView* render_target_views[4] = 
	{ 
		output_merger->GetRenderTargetView("GBufferPositionView"),
		output_merger->GetRenderTargetView("GBufferNormalView"),
		output_merger->GetRenderTargetView("GBufferDiffuseView"),
		output_merger->GetRenderTargetView("GBufferViewDirectionView")
	};
	deferred_context->OMSetRenderTargets(4, render_target_views, output_merger->GetDepthStencilView());
	deferred_context->OMSetDepthStencilState(output_merger->GetDepthStencilState("FirstPassState"), 0x01);
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kVertexShaderSlotWorldMatrix_, 1, world_matrix_constant_buffer_.GetAddressOf());
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->PSSetShaderResources(0, 1, player_mesh->texture_component_->GetTextureShaderResourceView("DiffuseView"));
	deferred_context->PSSetShaderResources(1, 1, player_mesh->texture_component_->GetTextureShaderResourceView("NormalView"));
	deferred_context->PSSetSamplers(0, 1, player_mesh->texture_component_->GetSampler());
	deferred_context->DrawIndexed(player_mesh->GetNumIndices(), 0, 0);

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

void Player::SetMoveSpeed(float move_speed)
{
	move_speed_ = move_speed;
}

void Player::Move(DirectX::FXMVECTOR move_vector)
{
	DirectX::XMVECTOR new_position = DirectX::XMVectorAdd(GetPosition(), DirectX::XMVectorScale(move_vector, move_speed_));
	SetPosition(new_position);
}
