#include "time_formatter.h"
#include "app_time.h"

#include <boost/format.hpp>

TimeFormatter::LocalTime TimeFormatter::localTimeFromString
    (const std::string& input_format, const std::string& str)
{
    boost::local_time::local_time_input_facet* input = 
        new boost::local_time::local_time_input_facet();
    std::stringstream ss;

    boost::local_time::local_date_time date = 
        boost::local_time::local_sec_clock::local_time(
            boost::local_time::time_zone_ptr());

    ss.imbue(std::locale(ss.getloc(), input));

    ss.str(str);
    input->format(input_format.c_str());

    ss >> date;

    return date;
}

TimeFormatter::GregorianDate TimeFormatter::dateFromString
    (const std::string& input_format, const std::string& str)
{
    return boost::gregorian::from_string(str);
}

TimeFormatter::PosixTime TimeFormatter::ptimeFromString
    (const std::string& input_format, const std::string& str)
{
    boost::posix_time::ptime ptime = boost::posix_time::time_from_string(str);
    
    return ptime;
}

long long TimeFormatter::millisFromString
    (const std::string& input_format, const std::string& str)
{
    boost::posix_time::ptime ptime = ptimeFromString(input_format, str);

    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::time_duration diff = ptime - epoch;

    return diff.total_milliseconds();
}

std::chrono::system_clock::time_point TimeFormatter::timepointFromString(
    const std::string& input_format, const std::string& str)
{
    long long millis = millisFromString(input_format, str);
    
    std::chrono::system_clock::time_point tp;
    AppTime::setTimeInMillis(millis, tp);
    
    return tp;
}

std::string TimeFormatter::stringFromLocalTime(TimeFormatter::LocalTime& local_time)
{
    boost::posix_time::ptime ptime = local_time.utc_time();
    
    return stringFromPTimeInternal(ptime);
}

std::string TimeFormatter::stringFromDate(TimeFormatter::GregorianDate& date)
{
    boost::posix_time::ptime ptime(date);
    
    return stringFromPTimeInternal(ptime);
}

std::string TimeFormatter::stringFromPTime(TimeFormatter::PosixTime& ptime)
{
    return stringFromPTimeInternal(ptime);
}

std::string TimeFormatter::stringFromMillis(long long millis)
{
    boost::posix_time::time_duration td = boost::posix_time::milliseconds(millis);
    boost::posix_time::ptime ptime(boost::gregorian::date(1970, 1, 1), td);

    return stringFromPTimeInternal(ptime);
}

std::string TimeFormatter::stringFromTimepoint(std::chrono::system_clock::time_point& tp)
{
    long long millis = AppTime::getTimeInMillis(tp);
    
    return stringFromMillis(millis);
}

std::tm TimeFormatter::tmFromLocalTime(LocalTime& local_time)
{
    boost::posix_time::ptime ptime = local_time.utc_time();
    std::tm tm;
    
    tmFromPTimeInternal(ptime, tm);
    
    return tm;
}

std::tm TimeFormatter::tmFromDate(GregorianDate& date)
{
    boost::posix_time::ptime ptime(date);
    std::tm tm;
    
    tmFromPTimeInternal(ptime, tm);
    
    return tm;
}

std::tm TimeFormatter::tmFromPTime(PosixTime& ptime)
{
    std::tm tm;
    tmFromPTimeInternal(ptime, tm);
    
    return tm;
}

std::tm TimeFormatter::tmFromMillis(long long millis)
{
    boost::posix_time::time_duration td = boost::posix_time::milliseconds(millis);
    boost::posix_time::ptime ptime(boost::gregorian::date(1970, 1, 1), td);
    std::tm tm;
    
    tmFromPTimeInternal(ptime, tm);
    
    return tm;
}

std::tm TimeFormatter::tmFromTimepoint(std::chrono::system_clock::time_point& tp)
{
    long long millis = AppTime::getTimeInMillis(tp);
    
    return tmFromMillis(millis);
}

// private member function
std::string TimeFormatter::stringFromPTimeInternal(PosixTime& ptime)
{
    std::tm tm = boost::posix_time::to_tm(ptime);

    std::string date_str = (boost::format("%04d/%02d/%02d") % 
        (1900 + tm.tm_year) % (tm.tm_mon + 1) % tm.tm_mday).str();
    date_str += " ";
    date_str += (boost::format("%02d:%02d:%02d") % tm.tm_hour % 
        tm.tm_min % tm.tm_sec).str().c_str();

    return date_str;
}

void TimeFormatter::tmFromPTimeInternal(PosixTime& ptime, std::tm& tm)
{
    tm = boost::posix_time::to_tm(ptime);
}
