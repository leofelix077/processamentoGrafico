#pragma once
#include <time.h>
//----------------------------------------------------------------------
//classe timer baseada nos slides do material
class Timer {
public:
	Timer::Timer() {
		reset();
	}
	void Timer::start() {
		begin = clock();
	}
	void Timer::finish() {
		end = clock();
	}
	void Timer::reset() {
		begin = 0;
		end = 0;
	}
	double Timer::getElapsedTimeMs() {
		double e = ((double)(end - begin)) / CLOCKS_PER_SEC;
		return e * 1000;
	}
	//----------------------------------------------------------------------
private:
	clock_t begin, end;
};

