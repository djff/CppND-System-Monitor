#include <string>
#include <cmath>
#include <iomanip>

#include "format.h"

#include <iostream>

using std::cout;

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  std::string string_time = "";
  long hour, minute, second;
  
  hour = floor((double) seconds/3600);
  minute = floor((double) (seconds-(hour*3600))/60);
  second = seconds - (hour*3600 + minute*60);
  
  std::ostringstream linestream;
  
  linestream << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << minute << ":" << std::setw(2) << second;

  return linestream.str(); 
}