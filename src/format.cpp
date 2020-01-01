#include "format.h"

#include <iostream>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  string formattedHours =
      hours < 9 ? string("0" + std::to_string(hours)) : std::to_string(hours);

  long minutes = (seconds % 3600) / 60;
  string formattedMinutes = minutes < 9 ? string("0" + std::to_string(minutes))
                                        : std::to_string(minutes);

  long second = (seconds % 3600) % 60;
  string formattedSeconds = second < 9 ? string("0" + std::to_string(second))
                                       : std::to_string(second);

  return formattedHours + ":" + formattedMinutes + ":" + formattedSeconds;
}