#pragma once
#include "InstanceObject.h"
#include "../Meshes/Vertices.h"
#include <vector>

struct BulletState
{
	bool active;
	float elapsed_time;
	DirectX::XMFLOAT3 init_position;
	DirectX::XMFLOAT3 init_direction;

	BulletState()
	{
		active = false;
		elapsed_time = 0.0f;
		init_position.x = 0.0f;
		init_position.y = 0.0f;
		init_position.z = 0.0f;
		init_direction.x = 0.0f;
		init_direction.y = 0.0f;
		init_direction.z = 0.0f;
	}
};

class BulletObject : public InstanceObject<BulletInstanceData>
{
private:
	std::vector<BulletState> bullet_active_manager_;

private:
	unsigned int GetAtiveCount() const;

public:
	virtual void Initialize() override;
	virtual void AnimateObject() override;

	unsigned int GetBulletCount() const;
	bool GetActiveOfIndex(unsigned int index);
	void SetActiveOfIndex(unsigned int index);
	void SetInitPositionOfIndex(unsigned int index, DirectX::XMFLOAT3 init_position);
	void SetInitDirectionOfIndex(unsigned int index, DirectX::XMFLOAT3 init_direction);
	BulletInstanceData* GetBulletDataOfIndex(unsigned int index);
	 
	void UpBulletSpeed();
	void DownBulletSpeed();

private:
	float bullet_peed_ = 100.0f;

};

