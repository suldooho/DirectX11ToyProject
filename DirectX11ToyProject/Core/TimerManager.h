#pragma once

class TimerManager
{
private:
	static TimerManager* instance_;

	TimerManager() = default;
	~TimerManager() = default;

public:
	TimerManager(const TimerManager&) = delete;
	TimerManager& operator=(const TimerManager&) = delete;

	static TimerManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new TimerManager();
		}

		return instance_;
	}

private:
	__int64 previous_time_;
	__int64 current_time_;
	__int64 count_per_sec_;
	float second_per_count_;
	float delta_time_;

private:
	void CaclulateFrameCount();

public:
	void Initialize();

	void Tick();

	float GetDeltaTime() const;
};

