#include "processor.h"
#include "linux_parser.h"
#include <fstream>
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  std::string attribute, line;
  int user, nice, system, idle, iowait, irq, softirq, steal;
  
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  if(filestream.is_open()){
  	// get first line of file
    std::getline(filestream, line);
    
    std::istringstream linestream(line);
    
    linestream >> attribute >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    
    if(attribute == "cpu"){
      // calculate cpu utilization
      int idle_time = iowait + idle; // get the cpu's idle time
      int non_idle_time = user + nice + system + irq + softirq + steal; // get the cpu non idle time
      
      int total_time = idle_time + non_idle_time; // get cpu's total utilization time
      
      int total_d = total_time - Processor::_prev_total; // the the time difference of previous calculation and current calculation
      
      int idle_d = idle_time - Processor::_prev_idle; 
      
      // update previous times
      Processor::_prev_total = total_time;
      Processor::_prev_idle = idle_time;
        
      auto result = (float) (total_d - idle_d)/total_d ;
            
      return result;
      
    }
  }
  
  return 0.0; 

}