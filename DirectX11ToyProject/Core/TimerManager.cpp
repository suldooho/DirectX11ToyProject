#include "TimerManager.h"
#include "../framework.h"

TimerManager* TimerManager::instance_ = nullptr;

void TimerManager::CaclulateFrameCount()
{
	static float time_elapsed = 0.0f;
	static unsigned int frame_count = 0;

	++frame_count;

	time_elapsed += delta_time_;

	if (1.0f <= time_elapsed)
	{
		time_elapsed = 0.0f;
		frame_count = 0;
	}
}

void TimerManager::Initialize()
{
	__int64 count_per_sec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&count_per_sec));
	second_per_count_ = 1.0f / static_cast<float>(count_per_sec);
}

void TimerManager::Tick()
{ 
	__int64 curr_time;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr_time));
	current_time_ = curr_time;

	delta_time_ = (current_time_ - previous_time_) * second_per_count_;

	previous_time_ = current_time_;

	CaclulateFrameCount();
}

float TimerManager::GetDeltaTime() const
{
	return delta_time_;
}
