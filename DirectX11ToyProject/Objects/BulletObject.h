#pragma once
#include "InstanceObject.h"
#include "../Meshes/Vertices.h"
#include <vector>

struct BulletState
{
	bool active;
	float elapsed_time;

	BulletState()
	{
		active = false;
		elapsed_time = 0.0f;
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
	BulletInstanceData* GetBulletDataOfIndex(unsigned int index);
	 

private:
	const float kSpeed_ = 5000.0f;

};

