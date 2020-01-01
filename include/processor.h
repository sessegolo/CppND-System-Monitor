#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  Processor()
      : _key(0.0),
        _user(0.0),
        _nice(0.0),
        _system(0.0),
        _idle(0.0),
        _iowait(0.0),
        _irq(0.0),
        _softirq(0.0),
        _steal(0.0),
        _guest(0.0),
        _guest_nice(0.0),
        _validData(false){};
  float Utilization();

 private:
  void readCpuData(const std::vector<std::string>& cpuData);
  float _key, _user, _nice, _system, _idle, _iowait, _irq, _softirq, _steal,
      _guest, _guest_nice;
  bool _validData;
};

#endif