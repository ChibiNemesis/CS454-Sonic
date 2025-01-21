#pragma once
#include <chrono>
class SystemClock {
private:
	std::chrono::high_resolution_clock clock;
	static SystemClock* singleton;
public:
	static auto Get(void) -> SystemClock&;
	uint64_t milli_secs(void) const;
	uint64_t micro_secs(void) const;
	uint64_t nano_secs(void) const;
};

uint64_t GetSystemTime(void);