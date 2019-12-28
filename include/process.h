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
  Process(const int pid) : _pid(pid), _uid(), _user(), 
               _command(), _cpuUtilization(), _ramUsage(), 
              _upTime(), _status(ProcessStatus::Running) 
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
  inline std::string Uid() { return _uid; }
  inline ProcessStatus Status() { return _status; } // i dont think I need this anymore

  // TODO: Declare any necessary private members
 private:
  void init();
  void processStatus();
  int _pid;
  std::string _uid;
  std::string _user;
  std::string _command;
  float _cpuUtilization;
  std::string _ramUsage;
  long int _upTime;
  ProcessStatus _status;
};

#endif