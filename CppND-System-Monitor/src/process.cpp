#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id) : pid(id) {}

int Process::Pid() {
  return pid;
}

float Process::CpuUtilization() const {
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  float totalJiffies = LinuxParser::Jiffies() * 1.f;
  float activeJiffies = LinuxParser::ActiveJiffies(pid) * 1.f;
  return fabs(totalJiffies) < 0.0005 ? 0 : (activeJiffies / totalJiffies);
}

string Process::Command() {
  std::string command = LinuxParser::Command(pid);
  if (command.length() > 40) {
    // truncate string at 40 chars
    command.erase(40, std::string::npos);
    command.append("...");
  }
  return command;
}

string Process::Ram() {
  return LinuxParser::Ram(pid);
}

string Process::User() {
  return LinuxParser::User(pid);
}

long int Process::UpTime() {
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid);
}

bool Process::operator<(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}
