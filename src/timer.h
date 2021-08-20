#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <QString>
#include <string>


class Timer
{
    std::chrono::steady_clock::time_point tpStart;
    std::chrono::steady_clock::time_point tpStop;
    bool stopped = false;

public:
    Timer()
    {
        using namespace std::chrono;
        tpStart = steady_clock::now();
    }

    void Start()
    {
        using namespace std::chrono;
        tpStart = steady_clock::now();
        stopped = false;
    }

    void Stop()
    {
        using namespace std::chrono;
        tpStop = steady_clock::now();
        stopped = true;
    }

    unsigned long long Get()
    {
        using namespace std::chrono;
        if (stopped)
            return duration_cast<milliseconds>(tpStop - tpStart).count();
        else return 0;
    }

    void Get(QString & getTime)
    {
        std::string text = std::to_string(Get());
        getTime.fromStdString(text);
    }

    void GetStop(QString & getTime)
    {
        Stop();
        Get(getTime);
    }
};

#endif // TIMER_H
