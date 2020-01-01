#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

namespace {
void setupProcesses(std::vector<Process>& processes) {
  processes.clear();
  for (int pid : LinuxParser::Pids()) {
    Process process(pid);
    processes.push_back(process);
  }
}
}  // namespace

void System::init() {
  _os = LinuxParser::OperatingSystem();
  _kernel = LinuxParser::Kernel();
  setupProcesses(_processes);
}

Processor& System::Cpu() { return _cpu; }

vector<Process>& System::Processes() {
  setupProcesses(_processes);
  return _processes;
}

std::string System::Kernel() { return _kernel; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return _os; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }