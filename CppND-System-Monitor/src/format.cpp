#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  long h = seconds / 3600;
  seconds = seconds % 3600;
  long m = seconds / 60;
  seconds = seconds % 60;
  std::stringstream strstream;
  std::string result;
  // https://en.cppreference.com/w/cpp/io/manip/setfill
  strstream << std::setfill('0') << std::setw(2) << h << ":";
  strstream << std::setfill('0') << std::setw(2) << m << ":";
  strstream << std::setfill('0') << std::setw(2) << seconds;
  strstream >> result;
  return result;
}