#pragma once

class DebugLog
{
private:
	static DebugLog* instance_;

	DebugLog() = default;
	~DebugLog() = default;

public:
	DebugLog(const DebugLog&) = delete;
	DebugLog& operator=(const DebugLog&) = delete;

	static DebugLog* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new DebugLog();
		}

		return instance_;
	}

public:
	void LogFormattedMessage(const char* format, ...);
};

