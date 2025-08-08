#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>



class LogReader
{
public:
    LogReader(const std::string& file_path,int buffersize = 10)
        : infile_(file_path), buffer_size_(buffersize)
    {
        if (file_path.empty())
        {
            throw std::invalid_argument("Log file path cannot be empty");
        }

        if (!infile_.is_open())
        {
            std::error_code ec(errno, std::generic_category());
            std::cerr << "Could not open file: " << ec.message() << '\n';

            std::cerr << "Error opening file: " << strerror(errno) << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl; 
          
            throw std::runtime_error("Failed to open log file");

        }
    }
    ~LogReader() 

    {
        if (infile_.is_open())
        {
            infile_.close();
        }
    }

    LogReader(const LogReader&) = delete; // Disable copy constructor
    LogReader& operator=(const LogReader&) = delete; // Disable copy assignment

    std::pair<int, const std::string &> read_next_line();
    void deleteProcessedLine();

private:
    void load_next_buffer(); // Loads the next 10 lines into the buffer
   

    std::ifstream infile_;
    int current_line_index_ = 0;      // Tracks the current line index
    std::vector<std::string> buffer_; // Buffer to store up to 10 lines
    bool eof_reached_ = false;
    int buffer_size_ = 10; // Number of lines to read at a time
};
