#include "processor.h"

#include "linux_parser.h"

void Processor::readCpuData(const std::vector<std::string>& cpuData) {
  if (cpuData.size() < 10) return;
  _user = std::stol(cpuData[LinuxParser::CPUStates::kUser_]);
  _nice = std::stol(cpuData[LinuxParser::CPUStates::kNice_]);
  _system = std::stol(cpuData[LinuxParser::CPUStates::kSystem_]);
  _idle = std::stol(cpuData[LinuxParser::CPUStates::kIdle_]);
  _iowait = std::stol(cpuData[LinuxParser::CPUStates::kIOwait_]);
  _irq = std::stol(cpuData[LinuxParser::CPUStates::kIRQ_]);
  _softirq = std::stol(cpuData[LinuxParser::CPUStates::kSoftIRQ_]);
  _steal = std::stol(cpuData[LinuxParser::CPUStates::kSteal_]);
  _guest = std::stol(cpuData[LinuxParser::CPUStates::kGuest_]);
  _guest_nice = std::stol(cpuData[LinuxParser::CPUStates::kGuestNice_]);
}

float Processor::Utilization() {
  float usage{0.0};
  std::vector<std::string> cpuData = LinuxParser::CpuUtilization();
  if (_validData) {
    // store previous values
    const float prevUser{_user}, prevNice{_nice}, prevSystem{_system},
        prevIdle{_idle}, prevIowait{_iowait}, prevIrq{_irq},
        prevSoftirq{_softirq},
        prevSteal{_steal};  // prevGuest{_guest}, prevGuestNice{_guest_nice};
                            // both not used

    // read new values
    readCpuData(cpuData);

    // math from Stack Overflow
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    const float totalPrevIdle = prevIdle + prevIowait;
    const float idle = _idle + _iowait;

    const float totalPrevNonIdle =
        prevUser + prevNice + prevSystem + prevIrq + prevSoftirq + prevSteal;
    const float nonIdle = _user + _nice + _system + _irq + _softirq + _steal;

    const float totalPrev = totalPrevIdle + totalPrevNonIdle;
    const float total = idle + nonIdle;

    const float deltaTotal = total - totalPrev;
    const float deltaIdle = idle - totalPrevIdle;
    
    usage = deltaTotal > 0 ? (deltaTotal - deltaIdle) / deltaTotal : 0.0;
  } else {
    // initialize member variables with current value
    readCpuData(cpuData);
    _validData = true;
  }
  return usage;
}
