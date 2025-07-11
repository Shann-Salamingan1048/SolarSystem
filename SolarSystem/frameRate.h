#include <chrono>
#include <thread>

class frameRate
{
public:
	frameRate() noexcept = default;

public:
	void sleep() const;
	void startlastTime();
	void startCurrTime();
	void startDeltaTime();

public:
	bool checkDeltaTime() const;

public:
	void setLastTime(const std::chrono::high_resolution_clock::time_point& currTime);
	std::chrono::high_resolution_clock::time_point getCurrTime() const;

private:
	constexpr static double m_targetFPS = 500.0;
	constexpr static double m_frameTime = 1.0 / m_targetFPS;

	double m_deltaTime;
	std::chrono::steady_clock::time_point m_lastTime;
	std::chrono::steady_clock::time_point m_currTime;

};