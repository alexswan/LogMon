
#include <iostream>
using namespace std;

#include "LogEntry.h"
#include "LogReader.h"
#include "LogReporter.h"
#include "LineProcess.h"


int main()
{
   std::cout << "==========================\n=  Starting Log Parser!\n==========================" << std::endl;
    try
    {
        LogReader   reader("./Logmon/src/logs.log");
  
        LogReporter    log("./LogMon/src/logreporter.log", LogReporter::Level::Warning);

        LineProcessor processor(reader, log);

   
        processor.process_lines();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error processing log lines: " << e.what() << std::endl;
        return 1; // Return a non-zero exit code on error
    }
   
    std::cout << "==========================\n=  Log processing completed successfully.\n==========================" << std::endl;
  
    return 0;
}