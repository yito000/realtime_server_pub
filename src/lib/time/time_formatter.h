#ifndef TIME_FORMATTER_H
#define TIME_FORMATTER_H

#include <string>
#include <boost/date_time.hpp>
#include <chrono>
#include <ctime>

class TimeFormatter
{
public:
    typedef boost::local_time::local_date_time LocalTime;
    typedef boost::gregorian::date GregorianDate;
    typedef boost::posix_time::ptime PosixTime;
    
    static LocalTime localTimeFromString
        (const std::string& input_format, const std::string& str);
    static GregorianDate dateFromString
        (const std::string& input_format, const std::string& str);
    static PosixTime ptimeFromString
        (const std::string& input_format, const std::string& str);
    static long long millisFromString(const std::string& input_format, 
        const std::string& str);
    static std::chrono::system_clock::time_point timepointFromString(
        const std::string& input_format, const std::string& str);
    
    static std::tm tmFromLocalTime(LocalTime& local_time);
    static std::tm tmFromDate(GregorianDate& date);
    static std::tm tmFromPTime(PosixTime& ptime);
    static std::tm tmFromMillis(long long millis);
    static std::tm tmFromTimepoint(std::chrono::system_clock::time_point& tp);
    
    static std::string stringFromLocalTime(LocalTime& local_time);
    static std::string stringFromDate(GregorianDate& date);
    static std::string stringFromPTime(PosixTime& ptime);
    static std::string stringFromMillis(long long millis);
    static std::string stringFromTimepoint(std::chrono::system_clock::time_point& tp);
    
private:
    static std::string stringFromPTimeInternal(PosixTime& ptime);
    static void tmFromPTimeInternal(PosixTime& ptime, std::tm& tm);
};

#endif
