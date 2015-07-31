#ifndef APP_TIME_H
#define APP_TIME_H

#include <chrono>

class AppTime
{
public:
    typedef std::chrono::system_clock::time_point Point;
    typedef std::chrono::system_clock::time_point::duration Duration;

    static Point now();
    static long long nowMillis();
    static long long getTimeInMillis(Point& tp);
    static void setTimeInMillis(long long millis, Point& tp);
};

#endif
