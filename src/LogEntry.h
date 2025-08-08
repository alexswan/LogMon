#pragma once

#include <string>
#include <iostream>

enum class LogType: std::uint8_t {
    START = 0,
    END
};



class LogEntry
{
public:

 
    //constructors
    LogEntry() noexcept 
        : timestamp_(-1), name_(""), tag_(LogType::START), pid_(-1) {}
 
    LogEntry(int timestamp, std::string name, LogType tag, int pid) noexcept;
 

    //static methods
    static LogEntry from_string(const std::string& line) noexcept;
    static int parse_timestamp(const std::string &datetime) noexcept;

    static std::string timestamp_to_time(int timestamp) noexcept;

    //getters
    int get_timestamp_sec() const noexcept { return timestamp_; }
    const std::string& get_name() const noexcept { return name_; }
    LogType get_tag() const noexcept { return tag_; }
    int get_pid() const noexcept { return pid_; }   
    
private:
    //data members

    //the timestamp is computed before storing as seconds
    int timestamp_;

    //the name of the process. could be chinese also
    std::string name_;

    //the log entry type (START/END)
    LogType tag_;

    //the process ID, as in Linux it is an integer.
    // it should be unique and rising.
    // but in the file can be stored in any order
    int pid_;

};

