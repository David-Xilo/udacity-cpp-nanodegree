#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long newTotalJiffies = LinuxParser::Jiffies();
  long newIdleJiffies = LinuxParser::IdleJiffies();
  float totalJiffies = newTotalJiffies * 1.f - prevTotal * 1.f;
  float idleJiffies = newIdleJiffies * 1.f - prevIdle * 1.f;
  prevTotal = newTotalJiffies;
  prevIdle = newIdleJiffies;
  return (totalJiffies - idleJiffies) / totalJiffies;
}