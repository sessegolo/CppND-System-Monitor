#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

class Process {
 public:
  Process(const int pid) : _pid(pid), _user(), _command() { init(); };

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool isRunning() const;
  bool operator<(Process const& a) const;

 private:
  void init();
  int _pid;
  std::string _user;
  std::string _command;
};

#endif