#pragma once
#include <chrono>

//Very simple timer class, abstracts away a ton of std::chrono:: etc nonsense that gets very hard to read.
class Timer
{
public:
	Timer();
	~Timer();

	int64_t ElapsedMs();

private:
	std::chrono::time_point<std::chrono::steady_clock> timerStart;
};

