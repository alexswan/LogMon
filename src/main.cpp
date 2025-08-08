
#include <iostream>
using namespace std;

#include "LogEntry.h"
#include "LogReporter.h"



int main()
{
    cout << "hello" << endl;

     std::string input = "12:34:56,example,START,1234";

     LogEntry entry = LogEntry::from_string(input);


     LogReporter reporter("LOGMON/out/log.txt");
     reporter.write(LogReporter::Level::Warning, "This is a warning message");
     reporter.write(LogReporter::Level::Error, "This is an error message");
     reporter.flush();

    return 0;
}