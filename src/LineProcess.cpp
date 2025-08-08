#include "LineProcess.h"




 void LineProcessor::process_lines()
{
    while (true)
    {
        auto [status, line] = reader_.read_next_line();

        if (status == -1)
        {
            break; // Exit the loop when the end of the file is reached
        }
        ProcessSingleLine(line);
        reader_.deleteProcessedLine();
    }


    ProcessLogs();
}


  void LineProcessor::ProcessSingleLine(const std::string& line)
    {
        LogEntry entry = LogEntry::from_string(line); 

        //if pid is invalid, skip the entry
        if (entry.get_pid() == -1)
        {
            std::cout << "Invalid PID in log entry: " << line << std::endl;
            return; // Skip invalid entries
        }
 
                
        if(entry.get_tag() == LogType::START)
        {
            //check if the entry is already in the map
            if (log_entries.find(entry.get_pid()) == log_entries.end())
            {
                log_entries[entry.get_pid()] = entry; // Store the LogEntry in the map with PID as key
            }
            else
            {
                log_.write(LogReporter::Level::Error, "Duplicate start log entry for PID: ", entry.get_pid());
            }
        }
        else if (entry.get_tag() == LogType::END)
        {
            auto start_entry_it = log_entries.find(entry.get_pid());

            if (start_entry_it != log_entries.end())
            {

                // if we found a matching start.
                if(start_entry_it->second.get_tag() == LogType::START)
                {
                    //calculate the duration
                    int duration = entry.get_timestamp_sec() - start_entry_it->second.get_timestamp_sec();

                    // if duration is negative, it means the end log entry is before the start log entry
                    /*if (duration < 0)
                    {
                        log.write(LogReporter::Level::Error, "End log entry before start for PID: ", entry.get_pid(), ", duration: ", duration, " seconds");
                    }
                    else */
                    if (duration > ERROR_10_MIN) // if duration > 10 minutes print an error
                    {
                        log_.write(LogReporter::Level::Error, "Duration exceeds 10 minutes for PID: ", entry.get_pid(), ", duration: ", duration, " seconds");
                    }
                    else if (duration > WARNING_5_MIN) // if duration > 5 minutes print a warning
                    {
                        log_.write(LogReporter::Level::Warning, "Duration exceeds 5 minutes for PID: ", entry.get_pid(), ", duration: ", duration, " seconds");
                    }

                    log_entries.erase(start_entry_it);
                }
                else
                {
                    log_.write(LogReporter::Level::Error, "End log entry without matching start: ", entry.get_name());
                }
            }       
        }


    }




void   LineProcessor:: ProcessLogs()
{
     std::cout << "Log parsing completed successfully!" << std::endl;
    
    std::cout << "Total unmatched log entries: " << log_entries.size() << std::endl;
    //list all log entries
    for (const auto& entry : log_entries)
    {
        std::cout << "PID: " << entry.first 
                  << ", Name: " << entry.second.get_name()
                  << ", Timestamp: " << LogEntry::timestamp_to_time(entry.second.get_timestamp_sec())
                  << ", Tag: " << (entry.second.get_tag() == LogType::START ? "START" : "END") 
                  << std::endl;
    }
}


