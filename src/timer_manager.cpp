#include "timer_manager.h"

std::list<unsigned long long> timer_manager::m_times;
Timer timer_manager::m_timer;

timer_manager::timer_manager()
{

}

void timer_manager::start()
{
    m_timer.Start();
}

void timer_manager::stop()
{
    m_timer.Stop();
    m_times.push_back(m_timer.Get());
}

void timer_manager::reset()
{
    m_times.clear();
}

void timer_manager::get_time(QString &time_text, const unsigned long long &time)
{
    if (time > 0)
    {
        m_timer.Get(time_text, time);
    }
    else
    {
        unsigned long long time_ = 0;
        for (unsigned long long & t : m_times)
        {
            time_ += t;
        }
        m_timer.Get(time_text, time_);
    }
}

void timer_manager::get_time(unsigned long long &time, bool add)
{
    if (add == false) time = 0;
    for (unsigned long long & t : m_times)
    {
        time += t;
    }
}
