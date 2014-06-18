#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include <utility>
#include <stdio.h>

enum timer_scale {
	timer_raw,  /* pure performance counter */
	timer_micro /* values in microseconds */
};

#define MICRO_PER_SECOND 1000000
#define APP_START "appStartup"

class hrtime
{

public:
	hrtime(void) {}
	~hrtime(void) {}

	static void hrtime::init(void)
	{
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		m_frequency = f.QuadPart;
		printf("init hrtime with frequency: %llu\n", m_frequency);
	}

	static std::string hrtime::elapsed(void) 
	{
		return std::to_string(getDiff(APP_START, "now", timer_micro));
	}

	static long long hrtime::getDiff(std::string timerNameStart, std::string timerNameEnd, timer_scale scale)
	{
		long long start = getTimer(timerNameStart, timer_raw);
		long long end = getTimer(timerNameEnd, timer_raw);
		
		if (scale == timer_raw) { return (end - start); }
		if (scale == timer_micro) { return (((end - start) * MICRO_PER_SECOND) / m_frequency); }

		/* invalid scale */
		return 0;
	}

	static long long hrtime::getTimer(std::string timerName, timer_scale scale)
	{
		if (timerName == "now") {
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);

			if (scale == timer_raw) { return t.QuadPart; }
			if (scale == timer_micro) { 
				printf("now (us) called: %llu\n", ((t.QuadPart * MICRO_PER_SECOND) / m_frequency));
				return ((t.QuadPart * MICRO_PER_SECOND) / m_frequency); }
		}

		/* search for existing timer match */
		for (auto i = Timers.begin(); i != Timers.end(); ++i) {
			if (i->first == timerName) { printf("Found existing timer : %s (%llu)\n", timerName.c_str(), i->second); return i->second; }			
		}

		/* couldn't find timer, just return zero */
		return 0;
	}

	static void hrtime::setTimer(std::string timerName)
	{
		/* search for existing definition (update if found) */
		for (auto i = Timers.begin(); i != Timers.end(); ++i) {
			if (i->first == timerName) {
				i->second = getTimer("now", timer_raw);
				printf("updated %s\n", timerName.c_str());
				return;
			}
		}

		/* new timer, we need to add it */
		printf("adding new timer %s\n", timerName.c_str());
		std::pair<std::string, long long> TMElement = { timerName, getTimer("now", timer_raw) };
		Timers.insert(TMElement);
	}

private:
	static std::map<std::string, long long> Timers;
	static long long m_frequency;
};