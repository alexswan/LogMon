
#pragma once

#include <string>
#include <fstream>
#include <sstream>

using namespace std;


class LogReporter
{
public:
    enum class Level
    {
        Warning = 0,
        Error
    };

    LogReporter(const std::string& logfilename,
                Level initial_level = Level::Error,
                std::size_t max_buffer_lines = 500)

        : outfile_(logfilename,std::ofstream::out |std::ios::app),
          level_(initial_level),
          max_buffer_lines_(max_buffer_lines),
          buffer_()
    {
        if(logfilename.empty())
        {
            throw std::invalid_argument("Log filename cannot be empty");
        }
        else if (outfile_.fail())
        {
            throw std::runtime_error("Failed to open log file: " + logfilename);
        }
        else if (!outfile_)
        {
            throw std::runtime_error("Log file stream is not valid: " + logfilename);
        }
        // Open the log file or initialize logging system
        // For example, you might want to open a file stream here
    }

        template<typename... Args>
        void write(Level lvl, Args&&... args)
        {
            if (static_cast<int>(lvl) < static_cast<int>(level_))
                return;                           // too low-priority: skip

                
                    // Build the line in memory first.
            std::ostringstream oss;
            oss << (lvl == Level::Warning ? "[WARNING] " : "[ERROR] ");
            (oss << ... << std::forward<Args>(args));

            buffer_.emplace_back(std::move(oss).str());

            if (buffer_.size() >= max_buffer_lines_)
                flush();    
        }

         /// Manually flush any buffered lines to disk.
        void flush();

        ~LogReporter()
        {
            if (outfile_.is_open())
            {
                flush(); // Ensure all buffered lines are written before closing
                outfile_.close();
            }
    } 
    
        /// Change the minimum level that will actually be recorded.
    void set_level(Level lvl) noexcept { level_ = lvl; }

 

private:
    //the output file
    std::ofstream outfile_;
    //the logging verbosity: warning or "warnings and errors"
    Level level_;

    //how many lines do we read at once?
    const std::size_t        max_buffer_lines_;

    //the buffer where we read the lines
    std::vector<std::string> buffer_;
};