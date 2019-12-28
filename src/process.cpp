#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::init() {
    _command = LinuxParser::Command(_pid);
    _uid = LinuxParser::Uid(_pid);
    _user = LinuxParser::User(_pid);
    _upTime = LinuxParser::UpTime(_pid);
    _ramUsage = LinuxParser::Ram(_pid);
    processStatus();
}

int Process::Pid() { return _pid; }

float Process::CpuUtilization() { return _cpuUtilization; }

string Process::Command() { return _command; }

string Process::Ram() { return _ramUsage; }

string Process::User() { return _user; }

long int Process::UpTime() { return _upTime; }

bool Process::isRunning() const { return _status == ProcessStatus::Running; }

// TODO: Sort by the PID?
bool Process::operator<(Process const& a) const { 
    return _pid < a._pid;
}

void Process::processStatus() {
    std::string status = LinuxParser::ProcessStatus(_pid);
    
    if(status == "S") _status = ProcessStatus::Sleeping;
    else if(status == "R") _status = ProcessStatus::Running;
    else if(status == "D") _status = ProcessStatus::DiskSleep;
    else if(status == "T") _status = ProcessStatus::Stopped;
    else if(status == "t") _status = ProcessStatus::TracingStop;
    else if(status == "Z") _status = ProcessStatus::Zombie;
    else if(status == "X") _status = ProcessStatus::Dead;
    else _status = ProcessStatus::Sleeping; // Sleeping by default, in case of unexpected/older states
}