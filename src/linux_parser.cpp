#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
float LinuxParser::MemoryUtilization() { 
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

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
	return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::string gabbage, line;
  long utime, stime, cutime, cstime, starttime;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if(filestream.is_open()){
  	std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=0; i<23; i++){
    	if(i==14){linestream >> utime;}
      	else if(i==15){linestream >> stime;}
      	else if(i==16){linestream >> cutime;}
      	else if(i==17){linestream >> cstime;}
      	else if(i==22){linestream >> starttime; break;}
      	else{linestream >> gabbage;}
    }
    
    auto total_time = utime + stime + cutime + cstime ; // adding child jiffies because they are part of the process.
    
    return (total_time) / sysconf(_SC_CLK_TCK);
    
  }
  return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::string attribute, line;
  long user, nice, system, irq, softirq, steal;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(filestream.is_open()){
  	std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> attribute;
    for(int i=0; i<10; i++){
    	if(i==0){linestream >> user;}
      	else if(i==1){linestream >> nice;}
      	else if(i==2){linestream >> system;}
      	else if(i==5){linestream >> irq;}
      	else if(i==6){linestream >> softirq;}
      	else if(i==7){linestream >> steal; break;}
      	else{linestream >> attribute;}
    }
    return (user + nice + system + irq + softirq + steal);
  }
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::string attribute, line;
  long idle, iowait;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
   	linestream >> attribute;
    for(int i=0; i<10; i++){
    	if(i==3){
        	linestream >> idle;
        } else if (i == 4){
          	linestream >> iowait;
          	break;
        } else{
        	linestream >> attribute;
        }
    }
    return idle + iowait;
  }
  return 0; 
}

// TODO: Read and return CPU utilization
//vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string attribute, line;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> attribute >> value;
      if(attribute == "processes") return value;
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
	string attribute, line;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> attribute >> value;
      if(attribute == "procs_running") return value;
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    getline(filestream, line);
    return line;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string attribute, line;
  int ram_value;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> attribute >> ram_value;
      if(attribute == "VmSize"){
      	return to_string(ram_value/1000);
      }
    }
  } 
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string attribute, uid, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if(filestream.is_open()){
  	while(getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> attribute >> uid;
      if(attribute == "Uid:"){
      	return uid;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string uid = LinuxParser::Uid(pid);
  
  std::string user, x, uid_local, line;
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
  	while(getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid_local;
      if(uid == uid_local){
      	return user;
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string line, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if(filestream.is_open()){
    getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=0; i<22; i++){
      linestream >> value;
      if (i == 21){
        std::string::size_type sz;
        long int result = std::stol (value, &sz);
      	return result;
      }
    }
  }
  return 0; 
}

