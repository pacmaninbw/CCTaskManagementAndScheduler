#ifndef CC_UTILITY_TIMER_H_
#define CC_UTILITY_TIMER_H_

/*
 * Chernick Consulting Utility timer class.
 * Encapsulates execution timing for all or parts of a program.
 */

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string_view>

class UtilityTimer
{
public:
	using clock = std::chrono::steady_clock;

	void startTimer() noexcept
	{
		start = clock::now();
	}
	void stopTimerAndReport(std::string_view whatIsBeingTimed) noexcept
	{
		clock::time_point end = clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		double ElapsedTimeForOutPut = elapsed_seconds.count();

		using std::chrono::system_clock;
		auto const now = system_clock::to_time_t(system_clock::now());
		std::clog << "finished " << whatIsBeingTimed << std::put_time(std::localtime(&now), "%c")
			  << "\nelapsed time in seconds: " << ElapsedTimeForOutPut << "\n\n\n";
	}

private:
	clock::time_point start = clock::now();
};

#endif // CC_UTILITY_TIMER_H_
