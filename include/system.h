#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System() : _memoryUtilization(0.0), _upTime(0.0), _kernel(), _os() { init(); };
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();
  std::string OperatingSystem();

 private:
  void init();
  Processor _cpu = {};
  std::vector<Process> _processes = {};
  float _memoryUtilization;
  long _upTime;
  std::string _kernel;
  std::string _os;
};

#endif