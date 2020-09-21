#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::cout;

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  cpu_ = Processor();
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  std::string attribute, line;
  int total_memory, used_memory;
  
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  
  if(filestream.is_open()){
  	while(getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> attribute;
      if(attribute == "MemFree:"){
        linestream >> used_memory;
        return (float) (total_memory - used_memory) / total_memory;
      }
      linestream >> total_memory;
    }
  }
  return 0.0; 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  std::string line;
  long system_uptime;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  
  if(filestream.is_open()){
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> system_uptime;
    
    return system_uptime;
  }
  return 0; 
}


