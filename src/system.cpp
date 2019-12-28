#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <numeric>

using std::set;
using std::size_t;
using std::string;
using std::vector;

namespace {
    void setupProcesses(std::vector<Process>& processes) {
        for (int pid : LinuxParser::Pids()) {
            Process process(pid);
            processes.push_back(process);
        }
    }

    //int runningProcesses(const std::vector<Process>& processes) {
    //    return std::accumulate(processes.begin(), processes.end(), 0, [&](int acc, Process proc) -> int {
    //        return proc.isRunning() ? acc+1 : acc;
    //    });
    //}
}

void System::init() {
    _upTime = LinuxParser::UpTime();
    _memoryUtilization = LinuxParser::MemoryUtilization();
    _os = LinuxParser::OperatingSystem();
    _kernel = LinuxParser::Kernel();
    setupProcesses(_processes);
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return _cpu; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return _processes; }

std::string System::Kernel() { return _kernel; }

float System::MemoryUtilization() { return _memoryUtilization; }

std::string System::OperatingSystem() { return _os; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return _upTime; }