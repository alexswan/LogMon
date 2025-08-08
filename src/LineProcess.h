
#pragma once


#include "LogEntry.h"
#include "LogReader.h"
#include "LogReporter.h"

class LineProcessor
{
public:


    LineProcessor(LogReader& reader, LogReporter& log)
        : reader_(reader), log_(log){}
    ~LineProcessor() = default;

    // Process all lines from the log reader
    void process_lines();

private:
   

    // Process a single line from the log
    void ProcessSingleLine(const std::string &line);

    // Process the log entries that are not paired: the output is written to the screen
    void ProcessLogs();


    //references to the log reader and log reporter.
    //we presume they are valid for the lifetime of the LineProcessor
    LogReader& reader_;
    LogReporter& log_;

    const int ERROR_10_MIN = 600;
    const int WARNING_5_MIN = 300;

    //this map store all the log entries not processed.
    //if a pair is found with same PID it will be processed and removed from the map

    std::unordered_map<int, LogEntry> log_entries; // Map to store LogEntry and its PID
};
