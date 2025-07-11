#include "frameRate.h"

bool frameRate::checkDeltaTime() const
{
	return m_deltaTime >= m_frameTime;
}

void frameRate::startlastTime() 
{
	m_lastTime = std::chrono::high_resolution_clock::now();
}
void frameRate::startCurrTime() 
{
	m_currTime = std::chrono::high_resolution_clock::now();
}
void frameRate::startDeltaTime() 
{
	m_deltaTime =  std::chrono::duration<double>(m_currTime - m_lastTime).count();
}

void frameRate::sleep() const
{
	auto sleepTime = std::chrono::duration<double>(m_frameTime - m_deltaTime);
	std::this_thread::sleep_for(sleepTime);
}
void frameRate::setLastTime(const std::chrono::high_resolution_clock::time_point& currTime)
{
	m_lastTime = currTime;
}
std::chrono::high_resolution_clock::time_point frameRate::getCurrTime() const
{
	return m_currTime;
}