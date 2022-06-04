#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
}

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

float LinuxParser::MemoryUtilization() {
  float memTotal = findValueByKey<float>(filterMemTotalString, kMeminfoFilename);
  float memFree = findValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime() {
  return getValueOfFile<long>(kUptimeFilename);
}

std::vector<long> GetCpuUtilAsLongVector(std::vector<string> cpuUtil) {
  std::vector<long> cpu;
  // https://stackoverflow.com/questions/15619696/convert-vectorstring-to-vectorint-c-win32
  std::transform(cpuUtil.begin(), cpuUtil.end(), std::back_inserter(cpu),
                 [](const std::string& str) { return std::stol(str); });
  return cpu;
}

long LinuxParser::Jiffies() {
  std::vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  std::vector<long> cpu = GetCpuUtilAsLongVector(cpuUtilization);
  // https://stackoverflow.com/questions/3221812/how-to-sum-up-elements-of-a-c-vector
  return std::accumulate(cpu.begin(), cpu.end(), 0);
}

long LinuxParser::ActiveJiffies(int pid) {
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  long activeJiffies = 0;
  string line, key;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> key && count <= 17) {
        ++count;
        if (count >= 14 && count <= 17) {
          activeJiffies += std::stol(key);
        }
      }
    }
  }
  return activeJiffies;
}

bool IsActiveJiffie(int i) {
  return i != LinuxParser::CPUStates::kIdle_ && i != LinuxParser::CPUStates::kIOwait_;
}

long LinuxParser::ActiveJiffies() {
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  std::vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  std::vector<long> cpu = GetCpuUtilAsLongVector(cpuUtilization);
  long sum = 0;
  for (int i = kUser_; i <= kGuestNice_; ++i) {
    if (IsActiveJiffie(i)) {
      sum += cpu[i];
    }
  }
  return sum;
}

long LinuxParser::IdleJiffies() {
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  std::vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  std::vector<long> cpu = GetCpuUtilAsLongVector(cpuUtilization);
  long sum = 0;
  for (int i = kUser_; i <= kGuestNice_; ++i) {
    if (!IsActiveJiffie(i)) {
      sum += cpu[i];
    }
  }
  return sum;
}

vector<string> LinuxParser::CpuUtilization() {
  std::vector<string> cpu;
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && key != filterCpu) {
      std::istringstream linestream(line);
      linestream >> key;
      trim(key);
      if (key == filterCpu) {
        string value;
        while (linestream >> value) {
          cpu.push_back(value);
        }
      }
    }
  }
  return cpu;
}

int LinuxParser::TotalProcesses() {
  return findValueByKey<int>(filterProcesses, kStatFilename);
}

int LinuxParser::RunningProcesses() {
  return findValueByKey<int>(filterRunningProcesses, kStatFilename);
}

string LinuxParser::Command(int pid) {
  return getValueOfFile<string>(std::to_string(pid) + kCmdlineFilename);
}

string LinuxParser::Ram(int pid) {
  // using "VmRSS" instead of "VmSize" as suggested in the Udacity review
  // VmSize is the sum of all the virtual memory. VmRSS is the exact physical
  // memory being used as part of a Physical RAM. -> check man pages for more info
  // https://man7.org/linux/man-pages/man5/proc.5.html
  long valueKb = findValueByKey<long>(filterProcMem, std::to_string(pid) + kStatusFilename);
  // using long to have an int number, so the decimals don't occupy too much space
  return std::to_string(valueKb / 1000);
}

string LinuxParser::Uid(int pid) {
  return findValueByKey<string>(filterUID, std::to_string(pid) + kStatusFilename);
}

string LinuxParser::User(int pid) {
  string line, key, value, name;
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line) && value.empty()) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> key && count <= 3) {
        ++count;
        if (count == 1) {
          name = key;
        }
        if (count == 3 && key == uid) {
          return name;
        }
      }
    }
  }
  return "";
}

long LinuxParser::UpTime(int pid) {
  // https://man7.org/linux/man-pages/man5/proc.5.html
  string line, value;
  long clockTicks;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      int count = 0;
      while (linestream >> value && count <= 22) {
        ++count;
        if (count == 22) {
          clockTicks = std::stol(value);
        }
      }
    }
  }
  return clockTicks / sysconf(_SC_CLK_TCK);
}
