#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <experimental/filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace {
string readProcStat(const string keyToFind) {
  string key, val{"0"};
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    string line;
    while (std::getline(stream, line)) {
      if (line.find(keyToFind) != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> val;
        stream.close();
        return val;
      }
    }
  }
  stream.close();
  return val;
}

string readProcStat(const int pid, const int indexToRead) {
  string val{"0.0"};
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    string line;
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i < indexToRead; ++i) {
        linestream >> val;
      }
    }
  }
  stream.close();
  return val;
}
}  // namespace

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
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// Udacity Workspace fails to compile the following code
// Due to missing #include <filesystem>
/*vector<int> LinuxParser::Pids() {
  vector<int> pids{};
  std::experimental::filesystem::path procDirPath(kProcDirectory);
  for (const std::experimental::filesystem::directory_entry& dir :
       std::experimental::filesystem::directory_iterator(procDirPath)) {
    if (std::experimental::filesystem::is_directory(dir.path())) {
      string filename = dir.path().filename();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        pids.emplace_back(stoi(filename));
      }
    }
  }
  return pids;
}*/

// Code re-implemented using std::experimental::filesystem
// To be able to compile in Udacity Workspace
vector<int> LinuxParser::Pids() {
  vector<int> pids{};
  std::experimental::filesystem::path procDirPath(kProcDirectory);
  for (const std::experimental::filesystem::directory_entry& dir :
       std::experimental::filesystem::directory_iterator(procDirPath)) {
    if (std::experimental::filesystem::is_directory(dir.path())) {
      string filename = dir.path().filename();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        pids.emplace_back(stoi(filename));
      }
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string memTotal, memFree;
  if (stream.is_open()) {
    string line;
    string key, val;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if (line.find("MemTotal") != string::npos) {
        memTotal = val;
      } else if (line.find("MemFree") != string::npos) {
        memFree = val;
      } else {
        break;
      }
    }
  }
  stream.close();
  long total = stol(memTotal);
  long free = stol(memFree);
  return (total - free) / (total * 1.0);
}

long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string uptime, idle;
  if (stream.is_open()) {
    string line;
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> idle;
    }
  }
  stream.close();
  return stol(uptime);
}

// Jiffies are not used anymore on Kernels 2.6+
/*long LinuxParser::Jiffies() { return 0; }

// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() { return 0; }

long LinuxParser::IdleJiffies() { return 0; } */

vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line;
    while (std::getline(stream, line)) {
      if (line.find("cpu") != string::npos) {
        std::istringstream linestream(line);
        string key, user, nice, system, idle, iowait, irq, softirq, steal,
            guest, guest_nice;
        linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >>
            softirq >> steal >> guest >> guest_nice;
        vector<string> cpuData{user, nice,    system, idle,  iowait,
                               irq,  softirq, steal,  guest, guest_nice};
        stream.close();
        return cpuData;
      }
    }
  }
  stream.close();
  return {};
}

int LinuxParser::TotalProcesses() {
  string val = readProcStat("processes");
  return stoi(val);
}

int LinuxParser::RunningProcesses() {
  string val = readProcStat("procs_running");
  return stoi(val);
}

float LinuxParser::CpuUtilization(const int pid) {
  long systemUptime = UpTime();
  float clk = sysconf(_SC_CLK_TCK) + 0.0;
  long userCodeTime = stol(readProcStat(pid, 14));
  long kernelCodeTime = stol(readProcStat(pid, 15));
  long childrenUserCodeTime = stol(readProcStat(pid, 16));
  long childrenKernelCodeTime = stol(readProcStat(pid, 17));
  float procUpTime = UpTime(pid);

  // Calculation
  long totalTime = userCodeTime + kernelCodeTime + childrenUserCodeTime +
                   childrenKernelCodeTime;
  long seconds = systemUptime - procUpTime;

  float usage = (totalTime / clk) / seconds;
  return usage;
}

string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  stream.close();
  return line;
}

string LinuxParser::Ram(int pid) {
  string ram{"0"};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    string line;
    while (std::getline(stream, line)) {
      if (line.find("VmSize:") != string::npos) {
        std::istringstream linestream(line);
        string key;
        linestream >> key >> ram;
        int ramInMB = stoi(ram) / 1024;  // Yes, I want it to be truncated
        stream.close();
        return to_string(ramInMB);
      }
    }
  }
  stream.close();
  return ram;
}

string LinuxParser::Uid(int pid) {
  string uid{"0"};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    string line;
    while (std::getline(stream, line)) {
      if (line.find("Uid:") != string::npos) {
        std::istringstream linestream(line);
        string key;
        linestream >> key >> uid;
        stream.close();
        return uid;
      }
    }
  }
  stream.close();
  return uid;
}

string LinuxParser::User(int pid) {
  string user;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    string line;
    string uid = Uid(pid);
    string expression{"(.*):(.*):" + uid + "(.*)"};
    std::regex regexp(expression);
    while (std::getline(stream, line)) {
      if (std::regex_match(line, regexp)) {
        user = line.substr(0, line.find(":"));
        stream.close();
        return user;
      }
    }
  }
  stream.close();
  return user;
}

long LinuxParser::UpTime(int pid) {
  string procUpTime = readProcStat(pid, 22);
  long timeInTicks = stol(procUpTime);
  timeInTicks = timeInTicks / sysconf(_SC_CLK_TCK);
  long systemUptime = UpTime();
  long elapsedSeconds = systemUptime - timeInTicks;
  return elapsedSeconds;
}