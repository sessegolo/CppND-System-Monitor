#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
#include <regex>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  // read /proc/uptime
  // two values in seconds
  //the uptime of the system (including time spent in suspend) 
  //and the amountof time spent in the idle process.
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

string LinuxParser::ReadProcStat(const string keyToFind) {
  string key, val{"0"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    string line;
    while(std::getline(stream, line)) {
      if(line.find(keyToFind) != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> val;
        return val;
      }
    }
  }
  return val;
}

int LinuxParser::TotalProcesses() {
  string val = ReadProcStat("processes");
  return stoi(val);
}

int LinuxParser::RunningProcesses() {
  string val = ReadProcStat("procs_running");
  return stoi(val);
}

string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      return line;
    }
  }
  return line;
}

string LinuxParser::Ram(int pid) {
  string ram{"0"};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    string line;
    while(std::getline(stream, line)) {
      if(line.find("VmSize:") != string::npos) {
        std::istringstream linestream(line);
        string key;
        linestream >> key >> ram;
        float ramInMB = stoi(ram) / 1024.0;
        return to_string(ramInMB);
      }
    }
  }
  return ram;
}

string LinuxParser::Uid(int pid) {
  string uid{"0"};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    string line;
    while(std::getline(stream, line)) {
      if(line.find("Uid:") != string::npos) {
        std::istringstream linestream(line);
        string key;
        linestream >> key >> uid;
        return uid;
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string user;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    string line;
    string uid = Uid(pid);
    string expression{"(.*):(.*):" + uid + "(.*)"};
    std::regex regexp(expression);
    while(std::getline(stream, line)) {
      if(std::regex_match(line, regexp)) {
        user = line.substr(0, line.find(":"));
        return user;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string uptime{"0"};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    string line;
    if(std::getline(stream, line)) {
      std::istringstream linestream(line);
      for(int i = 0; i <= 22; ++i) {
        linestream >> uptime;
      }
      long timeInTicks = stol(uptime);
      timeInTicks = timeInTicks / sysconf(_SC_CLK_TCK);
      return timeInTicks;
    }
  }
  return stol(uptime);
}

string LinuxParser::ProcessStatus(const int pid) {
  string status{"S"}; // Sleeping by default
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    string line;
    while(std::getline(stream, line)) {
      if(line.find("State:") != string::npos) {
        std::istringstream linestream(line);
        string key;
        linestream >> key >> status;
        return status;
      }
    }
  }
  return status;
}
