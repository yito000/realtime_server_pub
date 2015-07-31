#include "app_time.h"

AppTime::Point AppTime::now()
{
    return std::chrono::system_clock::now();
}

long long AppTime::nowMillis()
{
    AppTime::Point p = AppTime::now();
    
    return AppTime::getTimeInMillis(p);
}

long long AppTime::getTimeInMillis(AppTime::Point& tp)
{
    return tp.time_since_epoch().count() / 1000 / 1000;
}

void AppTime::setTimeInMillis(long long millis, AppTime::Point& p)
{
    std::chrono::system_clock::time_point now = AppTime::now();
    std::chrono::system_clock::time_point::duration epoch_duration = now.time_since_epoch();
    
    std::chrono::system_clock::time_point epoch = now - epoch_duration;
    std::chrono::milliseconds millis_time = std::chrono::milliseconds(millis);
    
    std::chrono::system_clock::time_point::duration d(millis_time);

    p = epoch + d;
}
