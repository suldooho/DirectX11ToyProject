#include "EnemyObject.h"
#include "../framework.h"
#include "../Meshes/EnemyMesh.h"
#include "../FrameResources/EnemyShader.h"
#include "../FrameResources/OutputMerger.h"
#include <algorithm>

void EnemyObject::Initialize()
{
	// ���� �Լ��� ����Ͽ� 5x5 �׸���� �ν��Ͻ� ��ġ�� �ʱ�ȭ
	SetInitializeValue([]() -> std::unique_ptr<std::vector<EnemyInstanceData>> {
		unsigned int enemy_count = 20;
		int rand_max_position = 100;

		// unique_ptr�� vector ����
		auto instances = std::make_unique<std::vector<EnemyInstanceData>>(enemy_count);
		 
		for (auto& instance : *instances)
		{
			// ���� ��ġ �� ����
			float x = static_cast<float>(rand() % rand_max_position - rand_max_position / 2); // -50 ~ 50 ������ ���� x ��
			float y = static_cast<float>(rand() % rand_max_position);       // 0 ~ 100 ������ ���� y �� (0 �̻�)
			float z = static_cast<float>(rand() % rand_max_position - rand_max_position / 2); // -50 ~ 50 ������ ���� z ��

			// ���� ��� ���� (��ġ�� �̵� ��ķ� ����)
			instance.world0 = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
			instance.world1 = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			instance.world2 = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			instance.world3 = DirectX::XMFLOAT4(x, y, z, 1.0f); // ��ġ�� world3�� ����

			// ���� ���� �� ���� (0.0f ~ 1.0f ������ ������ ����)
			instance.color = DirectX::XMFLOAT3(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
		}

		return instances; // unique_ptr<std::vector<WorldInstanceData>> ��ȯ
		}); 

	CreateBuffer(); 

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferred_context;
	DeviceManager::GetInstance()->GetD3D11Device()->CreateDeferredContext(0, deferred_context.GetAddressOf());

	EnemyMesh* enemy_mesh = dynamic_cast<EnemyMesh*>(MeshesManager::GetInstance()->GetMesh<TextureVertex>("EnemyMesh"));
	if (enemy_mesh == nullptr)
	{
		throw std::string("EnemyMesh dynamic_cast Fail");
	}

	EnemyShader* enemy_shader = dynamic_cast<EnemyShader*>(FrameResourcesManager::GetInstance()->GetFrameResource("EnemyShader"));
	if (enemy_shader == nullptr)
	{
		throw std::string("EnemyShader dynamic_cast Fail");
	}

	OutputMerger* output_merger = dynamic_cast<OutputMerger*>(FrameResourcesManager::GetInstance()->GetFrameResource("OutputMerger"));
	if (output_merger == nullptr)
	{
		throw std::string("OutputMerger dynamic_cast Fail");
	}

	deferred_context->IASetPrimitiveTopology(enemy_mesh->GetPrimitiveTopology());
	ID3D11Buffer* buffer_pointers[2] = { enemy_mesh->GetVertexBuffer(), instance_vertex_buffer_.Get() };
	UINT strides[2] = { enemy_mesh->GetStride(), sizeof(EnemyInstanceData) };
	UINT offsets[2] = { enemy_mesh->GetOffset(), 0 };
	deferred_context->IASetVertexBuffers(0, 2, buffer_pointers, strides, offsets);
	deferred_context->IASetIndexBuffer(enemy_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deferred_context->IASetInputLayout(enemy_shader->GetInputLayout());
	deferred_context->VSSetShader(enemy_shader->GetVertexShader(), nullptr, 0);
	deferred_context->PSSetShader(enemy_shader->GetPixelShader(), nullptr, 0);
	deferred_context->RSSetViewports(1, output_merger->GetViewport());
	deferred_context->RSSetState(enemy_mesh->GetRasterizerState());
	ID3D11RenderTargetView* back_buffer_view = output_merger->GetRenderTargetView("BackBufferView");
	deferred_context->OMSetRenderTargets(1, &back_buffer_view, output_merger->GetDepthStencilView());
	deferred_context->OMSetDepthStencilState(output_merger->GetDepthStencilState("ForwardPassState"), 0x00);
	float blendFactors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deferred_context->OMSetBlendState(output_merger->GetBlendState("Transparent"), blendFactors, 0xffffffff);
	deferred_context->VSSetConstantBuffers(ObjectsManager::GetInstance()->kCameraShaderSlotWorldMatrix_, 1, ObjectsManager::GetInstance()->GetCameraConstantBuffer());
	deferred_context->PSSetShaderResources(0, 1, enemy_mesh->texture_component_->GetTextureShaderResourceView("DiffuseView")); 
	deferred_context->PSSetSamplers(0, 1, enemy_mesh->texture_component_->GetSampler());
	deferred_context->DrawIndexedInstanced(enemy_mesh->GetNumIndices(), instances_->size(), 0, 0, 0);

	deferred_context->FinishCommandList(true, command_list_.GetAddressOf());
}

void EnemyObject::AnimateObject()
{
	// �Ÿ� ����� ���� ���� �Լ� ����
	auto distanceFromCamera = [&](const EnemyInstanceData& instance) {
		DirectX::XMFLOAT3 instancePosition = { instance.world3.x, instance.world3.y, instance.world3.z };
		DirectX::XMVECTOR instancePosVec = DirectX::XMLoadFloat3(&instancePosition);
		DirectX::XMVECTOR distanceVec = DirectX::XMVectorSubtract(ObjectsManager::GetInstance()->GetCameraPosition(), instancePosVec);
		return DirectX::XMVectorGetX(DirectX::XMVector3Length(distanceVec));
		};

	// �Ÿ� �������� ����: �ָ� �ִ� ��ü�� 0��° �ε����� ������ �������� ����
	std::sort(instances_->begin(), instances_->end(),
		[&](const EnemyInstanceData& a, const EnemyInstanceData& b) {
			return distanceFromCamera(a) > distanceFromCamera(b);
		});

	for (auto& instance : *instances_)
	{
		// �ν��Ͻ��� ��ġ ���� (world3�� ��ġ�� ����)
		DirectX::XMFLOAT3 instancePosition = { instance.world3.x, instance.world3.y, instance.world3.z };

		// ī�޶� �ٶ󺸴� ���� ���
		DirectX::XMVECTOR instancePosVec = DirectX::XMLoadFloat3(&instancePosition);
		DirectX::XMVECTOR cameraPosVec = ObjectsManager::GetInstance()->GetCameraPosition();
		DirectX::XMVECTOR lookAtDir = DirectX::XMVectorSubtract(cameraPosVec, instancePosVec);
		lookAtDir = DirectX::XMVector3Normalize(lookAtDir);

		// ��ü�� ī�޶� ��ġ�� ����� upDir�� �������� ���
		DirectX::XMVECTOR defaultUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // �⺻ ���� ����
		DirectX::XMVECTOR rightDir = DirectX::XMVector3Cross(defaultUp, lookAtDir);
		rightDir = DirectX::XMVector3Normalize(rightDir);

		DirectX::XMVECTOR upDir = DirectX::XMVector3Cross(lookAtDir, rightDir);
		upDir = DirectX::XMVector3Normalize(upDir);

		// ���� ����� ���� ������Ʈ�Ͽ� ī�޶� ���ϰ� ����
		DirectX::XMStoreFloat4(&instance.world0, DirectX::XMVectorSetW(rightDir, 0.0f)); // X�� (right ����)
		DirectX::XMStoreFloat4(&instance.world1, DirectX::XMVectorSetW(upDir, 0.0f));   // Y�� (up ����)
		DirectX::XMStoreFloat4(&instance.world2, DirectX::XMVectorSetW(lookAtDir, 0.0f)); // Z�� (forward ����) 
	}
}
