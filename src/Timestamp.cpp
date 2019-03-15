#include "Timestamp.h"

#include <ctime>
#include <iomanip>
#include <iterator>
#include <sstream>

#pragma warning(push, 0) 
#include "cctz/civil_time.h"
#include "cctz/time_zone.h"
#include "cctz/zone_info_source.h"
#pragma warning(pop) 

using namespace std;

namespace {
  #include <Windows.h>

  long SystemTimeZoneOffSetMinutes() {
    TIME_ZONE_INFORMATION TimeZoneInfo;
    GetTimeZoneInformation(&TimeZoneInfo);
    return (long)TimeZoneInfo.Bias;
  }
}

Timestamp::Timestamp() {
  tp = std::chrono::system_clock::now();
}

Timestamp::Timestamp(const string& timestamp, const string& format) {
  const auto utc = cctz::utc_time_zone();
  if (!cctz::parse(format.c_str(), timestamp.c_str(), utc, &tp)) {
    tp = std::chrono::system_clock::now();
  }
}

bool Timestamp::operator<(const Timestamp& other) const {
  return tp < other.tp;
}

const string Timestamp::InUTCTime(const string& format) const {
  const auto utc = cctz::utc_time_zone();
  return cctz::format(format.c_str(), tp, utc).c_str();
}

const string Timestamp::InLocalTime(const string& format) const {
  cctz::sys_seconds offsetSeconds(SystemTimeZoneOffSetMinutes() * 60);
  const auto local = cctz::fixed_time_zone(-offsetSeconds);
  return cctz::format(format.c_str(), tp, local).c_str();
}

bool Timestamp::IsValid(const string& timestamp, const string& format) {
  std::chrono::system_clock::time_point tmpTp;
  const auto utc = cctz::utc_time_zone();
  return cctz::parse(format.c_str(), timestamp.c_str(), utc, &tmpTp);
}
