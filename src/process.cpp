#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::init() {
  _command = LinuxParser::Command(_pid);
  _user = LinuxParser::User(_pid);
}

int Process::Pid() { return _pid; }

float Process::CpuUtilization() { return LinuxParser::CpuUtilization(_pid); }

string Process::Command() { return _command; }

string Process::Ram() { return LinuxParser::Ram(_pid); }

string Process::User() { return _user; }

long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

bool Process::operator<(Process const& a) const { return _pid < a._pid; }