#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

enum class ProcessStatus {
    Running, //R
    Sleeping, //S
    DiskSleep, //D
    Stopped, //T
    TracingStop, //t as well...
    Zombie, // Z
    Dead //X
};

class Process {
 public:
  Process(const int pid) : _pid(pid), _user(), 
               _command(), _status(ProcessStatus::Sleeping) 
  {
    init();
  };

  int Pid();
  std::string User();                   
  std::string Command();                
  float CpuUtilization();               
  std::string Ram();                    
  long int UpTime();
  bool isRunning() const;
  bool operator<(Process const& a) const;

  // Inline functions
  inline ProcessStatus Status() { return _status; } // to be used later

 private:
  void init();
  void processStatus();
  int _pid;
  std::string _user;
  std::string _command;
  ProcessStatus _status;
};

#endif