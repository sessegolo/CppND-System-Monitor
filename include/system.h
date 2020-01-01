#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System() : _memoryUtilization(0.0), _kernel(), _os() { init(); };
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  void init();
  Processor _cpu = {};
  std::vector<Process> _processes = {};
  float _memoryUtilization;
  std::string _kernel;
  std::string _os;
};

#endif