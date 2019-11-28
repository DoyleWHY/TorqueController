#include<iostream>
#include<chrono>

#ifdef __DEBUG
	#define LOGFUNC LogROBO logger(__FUNCTION__);
#else
	#define LOGFUNC ;
#endif

class LogROBO
{
public:
	LogROBO(const char* func_name) :funcName(func_name)
	{
		t_begin = std::chrono::steady_clock::now();
		std::cout << "Enter " << funcName << std::endl;
	}

	~LogROBO()
	{
		std::cout << "Leave " << funcName << ". ";
		t_end = std::chrono::steady_clock::now();
		std::chrono::microseconds time_span = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_begin);
		std::cout << "Cost " << time_span.count() / 1000.0 << " ms" << std::endl;
	}

private:
	const char* funcName;
	std::chrono::steady_clock::time_point t_begin;
	std::chrono::steady_clock::time_point t_end;
};