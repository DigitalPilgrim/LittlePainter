#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <QString>
#include <list>
#include "timer.h"

class timer_manager
{
    static std::list<unsigned long long> m_times;
    static Timer m_timer;

    timer_manager();
public:

    static void start();
    static void stop();
    static void reset();
    static void get_time(QString & time_text, const unsigned long long &time = 0);
    static void get_time(unsigned long long &time, bool add = true);
};



#endif // TIMER_MANAGER_H
