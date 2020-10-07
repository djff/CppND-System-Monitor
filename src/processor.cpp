#include "processor.h"
#include "linux_parser.h"
#include <fstream>
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  	auto idle_time = LinuxParser::IdleJiffies();
  	auto non_idle_time = LinuxParser::ActiveJiffies();
  	
  	auto total_time = idle_time + non_idle_time;
  	auto total_d = total_time - Processor::_prev_total;
  	auto idle_d = idle_time - Processor::_prev_idle;
  	
  	// update previous times
  	Processor::_prev_total = total_time;
  	Processor::_prev_idle = idle_time;
  
  	auto result = (float) (total_d - idle_d)/total_d;
  
  	return result;
}