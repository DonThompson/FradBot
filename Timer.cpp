#include "Timer.h"

using namespace std;
using namespace std::chrono;

Timer::Timer()
	: timerStart(steady_clock::now())
{

}

Timer::~Timer()
{
}

int64_t Timer::ElapsedMs()
{
	milliseconds diff = duration_cast<milliseconds>(steady_clock::now() - timerStart);
	return diff.count();
}
