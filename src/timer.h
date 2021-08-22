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
            return duration_cast<nanoseconds>(tpStop - tpStart).count();
        else return 0;
    }

    void Get(QString & getTime, const unsigned long long &time = 0)
    {
        using ull = unsigned long long;
        ull nanoSeconds = 0; // Get()
        if (time > 0)
        {
            nanoSeconds = time;
        }
        else nanoSeconds = Get();
        long double sc = 1000000000.0; // sekund z nanosecond
        long double ms = 1000000; // milisekund z  nanosecond
        ull seconds = std::floor(static_cast<long double>(nanoSeconds) / sc);
        ull miliSeconds = std::floor(static_cast<long double>(nanoSeconds) / ms);

        miliSeconds = miliSeconds - (seconds * 1000);
        nanoSeconds = nanoSeconds - ((seconds * sc) + (miliSeconds * ms));

        std::string sSeconds = std::to_string(seconds);
        std::string sMiliSeconds = "";
        std::string sNanoSeconds = "";

        if (miliSeconds < 100) sMiliSeconds += "0";
        if (miliSeconds < 10)  sMiliSeconds += "0";
        sMiliSeconds += std::to_string(miliSeconds);

        ull nsRange = 100000;
        while (nsRange > 1)
        {
            if (nanoSeconds < nsRange) sNanoSeconds += "0";
            nsRange = nsRange / 10;
        }
        sNanoSeconds += std::to_string(nanoSeconds);

        sNanoSeconds.insert(3, " ");
        // vysledok = [ 1 s | 000 ms | 000 123 ns ]
        std::string vysledok = "[ "
                        + sSeconds + " s | "
                        + sMiliSeconds + " ms | "
                        + sNanoSeconds + " ns" + " ]";
        //std::string text = std::to_string(Get());

        getTime = QString::fromStdString(vysledok);
    }

    void GetStop(QString & getTime)
    {
        Stop();
        Get(getTime);
    }
};

#endif // TIMER_H
