#include "FloorObject.h"
#include "../framework.h"
#include "../Meshes/FloorMesh.h"
#include "../FrameResources/FloorShader.h"
#include "../FrameResources/OutputMerger.h"

void FloorObject::InitializeComponents()
{ 
	instance_component_ = std::make_unique<InstanceComponent<WorldInstanceData>>();

	// 람다 함수를 사용하여 5x5 그리드로 인스턴스 위치를 초기화
	instance_component_->Initialize([]() -> std::unique_ptr<std::vector<WorldInstanceData>> {
		unsigned int grid_size = 6;
		float spacing = 20.0f; // 인스턴스 간의 간격

		// unique_ptr로 vector 생성
		auto instances = std::make_unique<std::vector<WorldInstanceData>>(grid_size * grid_size);

		// 중앙 오프셋 계산: 타일들이 중앙 기준으로 배치되도록
		float half_grid_size = (grid_size - 1) / 2.0f;
		float x_offset = -half_grid_size * spacing;
		float z_offset = -half_grid_size * spacing;

		for (int i = 0; i < grid_size; ++i) {
			for (int j = 0; j < grid_size; ++j) {
				unsigned int index = i * grid_size + j;

				// 각 인스턴스의 월드 위치 설정
				(*instances)[index].world_matrix = {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					j * spacing + x_offset, -10.0f, i * spacing + z_offset, 1.0f // x, y, z, w
				};
			}
		}

		return instances; // unique_ptr<std::vector<WorldInstanceData>> 반환
		});
}

void FloorObject::Initialize()
{
	InitializeComponents();
	CreateConstantBuffer(); 

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
	ID3D11Buffer* buffer_pointers[2] = { floor_mesh->GetVertexBuffer(), instance_component_->GetInstanceVertexBuffer()};
	UINT strides[2] = { *(floor_mesh->GetStride()), instance_component_->GetStride() };
	UINT offsets[2] = { *(floor_mesh->GetOffset()), instance_component_->GetOffset() };
	deferred_context->IASetVertexBuffers(0, 2, buffer_pointers, strides, offsets);
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
	deferred_context->HSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->DSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer()); 
	deferred_context->PSSetShaderResources(0, 1, floor_mesh->texture_component_->GetTextureShaderResourceView("DiffuseView"));
	deferred_context->PSSetShaderResources(1, 1, floor_mesh->texture_component_->GetTextureShaderResourceView("NormalView"));
	deferred_context->DSSetShaderResources(2, 1, floor_mesh->texture_component_->GetTextureShaderResourceView("HeightView"));
	deferred_context->PSSetSamplers(0, 1, floor_mesh->texture_component_->GetSampler());
	deferred_context->DrawInstanced(floor_mesh->GetNumVertices(), instance_component_->GetInstanceCount(), 0, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());
}

void FloorObject::UpdateConstantBuffer()
{ 
}
	