#include "BulletObject.h" 
#include "../framework.h"
#include "../FrameResources/BulletShader.h"
#include "../FrameResources/OutputMerger.h"
#include "../Meshes/BulletMesh.h"
#include <algorithm>
#include <memory>

unsigned int BulletObject::GetAtiveCount() const
{ 
	unsigned int active_count = 0;

	for (unsigned int i = 0; i < bullet_active_manager_.size(); ++i)
	{
		if (bullet_active_manager_[i].active == true)
		{
			++active_count;
		}
		else
		{
			break;
		}
	}

	return active_count;
}

void BulletObject::Initialize()
{
	SetInitializeValue([]() -> std::unique_ptr<std::vector<BulletInstanceData>> {
		unsigned int bullet_count = 100; 
		 
		auto instances = std::make_unique<std::vector<BulletInstanceData>>(bullet_count);

		for (auto& instance : *instances)
		{  
			instance.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);  
			instance.prevPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);


			float red = static_cast<float>(rand()) / RAND_MAX + 0.7f;
			red = (red < 1.0f) ? red : 1.0f;
			float green = static_cast<float>(rand()) / RAND_MAX + 0.7f;
			green = (green < 1.0f) ? green : 1.0f;
			float blue = static_cast<float>(rand()) / RAND_MAX + 0.7f;
			blue = (blue < 1.0f) ? blue : 1.0f;

			instance.color = DirectX::XMFLOAT3(red, 0.0f, 0.0f);
		}

		return instances; 
		});

	bullet_active_manager_.resize(instances_->size());

	CreateBuffer();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());

	BulletMesh* bullet_mesh = dynamic_cast<BulletMesh*>(MeshesManager::GetInstance()->GetMesh<NullVertex>("BulletMesh"));
	if (bullet_mesh == nullptr)
	{
		throw std::string("BulletMesh dynamic_cast Fail");
	}

	BulletShader* bullet_shader = dynamic_cast<BulletShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("BulletShader"));
	if (bullet_shader == nullptr)
	{
		throw std::string("BulletShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	deferred_context->IASetPrimitiveTopology(bullet_mesh->GetPrimitiveTopology());
	deferred_context->IASetInputLayout(bullet_shader->GetInputLayout());
	UINT strides[] = { 0, sizeof(BulletInstanceData) };  
	UINT offsets[] = { 0, 0 };
	ID3D11Buffer* buffers[] = { nullptr, instance_vertex_buffer_.Get() }; 
	deferred_context->IASetVertexBuffers(0, 2, buffers, strides, offsets); 
	deferred_context->VSSetShader(bullet_shader->GetVertexShader(), nullptr, 0);
	deferred_context->GSSetShader(bullet_shader->GetGeometryShader(), nullptr, 0);
	deferred_context->PSSetShader(bullet_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport()); 
	ID3D11RenderTargetView* back_buffer_view = output_merger->GetRenderTargetView("BackBufferView");
	deferred_context->OMSetRenderTargets(1, &back_buffer_view, output_merger->GetDepthStencilView());
	deferred_context->OMSetDepthStencilState(output_merger->GetDepthStencilState("ForwardPassState"), 0x00);
	float blendFactors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deferred_context->OMSetBlendState(output_merger->GetBlendState("Transparent"), blendFactors, 0xffffffff);
	deferred_context->GSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->PSSetShaderResources(0, 1, bullet_mesh->texture_component_->GetTextureShaderResourceView("DiffuseView"));
	deferred_context->PSSetSamplers(0, 1, bullet_mesh->texture_component_->GetSampler());
	deferred_context->DrawInstanced(1, instances_->size(), 0, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());
}

void BulletObject::AnimateObject()
{    
	for (unsigned int i = 0; i < bullet_active_manager_.size(); ++i)
	{
		if (bullet_active_manager_[i].active)
		{
			bullet_active_manager_[i].elapsed_time += TimerManager::GetInstance()->GetDeltaTime();
			 
			if (3.0f <= bullet_active_manager_[i].elapsed_time)
			{
				bullet_active_manager_[i].active = false;
				bullet_active_manager_[i].elapsed_time = 0.0f;
				(*instances_)[i].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
				(*instances_)[i].prevPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); 
			}
			else
			{   
				DirectX::XMVECTOR init_position = DirectX::XMLoadFloat3(&bullet_active_manager_[i].init_position);

				DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&(*instances_)[i].position);
				DirectX::XMVECTOR velocity = ObjectsManager::GetInstance()->GetCameraLook();
				velocity = DirectX::XMVectorScale(velocity, bullet_peed_ * TimerManager::GetInstance()->GetDeltaTime());
				position = DirectX::XMVectorAdd(position, velocity);
				DirectX::XMStoreFloat3(&(*instances_)[i].position, position);
				  
				float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(position, init_position)));
				DirectX::XMVECTOR prev_position; 
				if (length <= 20.0f)
				{ 
					prev_position = init_position;
				}
				else
				{
					DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(init_position, position); 
					direction = DirectX::XMVector3Normalize(direction);

					prev_position = DirectX::XMVectorScale(direction, 20.0f);
					prev_position = DirectX::XMVectorAdd(position, prev_position); 
				} 
				DirectX::XMStoreFloat3(&(*instances_)[i].prevPosition, prev_position); 
			}
		}
	}

	std::vector<std::pair<BulletState&, BulletInstanceData&>> pairedData;

	// bullet_active_manager와 instances_를 페어로 묶어 새로운 벡터에 참조로 추가
	for (size_t i = 0; i < bullet_active_manager_.size(); ++i) {
		pairedData.emplace_back(bullet_active_manager_[i], (*instances_)[i]);
	}

	// active가 true인 항목을 앞쪽으로, false는 뒤쪽으로 정렬
	std::sort(pairedData.begin(), pairedData.end(), [](const auto& a, const auto& b) {
		return a.first.active > b.first.active;
		});

	// 참조를 이용해 데이터를 직접 bullet_active_manager와 instances_에 재배치
	for (size_t i = 0; i < pairedData.size(); ++i) {
		// 참조이므로 직접 대입 없이 정렬된 순서가 반영됩니다.
		bullet_active_manager_[i] = pairedData[i].first;
		(*instances_)[i] = pairedData[i].second;
	} 
}

unsigned int BulletObject::GetBulletCount() const
{
	return instances_->size();
}

bool BulletObject::GetActiveOfIndex(unsigned int index)
{
	return bullet_active_manager_[index].active;
}

void BulletObject::SetActiveOfIndex(unsigned int index)
{
	bullet_active_manager_[index].active = true;
}

void BulletObject::SetInitPositionOfIndex(unsigned int index, DirectX::XMFLOAT3 init_position)
{
	bullet_active_manager_[index].init_position = init_position;
}

BulletInstanceData* BulletObject::GetBulletDataOfIndex(unsigned int index)
{ 
	return &((*instances_)[index]);
}

void BulletObject::UpBulletSpeed()
{
	bullet_peed_ += 100.0f;
}

void BulletObject::DownBulletSpeed()
{
	bullet_peed_ -= 100.0f;

	if (bullet_peed_ <= 100.0f)
	{
		bullet_peed_ = 100.0f;
	}
}
  

