#include "LogReader.h"
#include <string>
#include <vector>
#include <iostream>
#include <utility>


void LogReader::load_next_buffer()
{
    buffer_.clear(); // Clear the buffer before loading new lines
  
    std::string line;
    int lines_to_read = buffer_size_;

    while (lines_to_read > 0 && std::getline(infile_, line))
    {
        buffer_.push_back(line); // Add the line to the buffer
        lines_to_read--;
    }

    if (infile_.eof())
    {
        eof_reached_ = true; // Mark that the end of the file has been reached
    }
}

std::pair<int, const std::string&> LogReader::read_next_line()
{
    static const std::string empty_line = ""; // Static empty string to return when no line is available

    if (buffer_.empty())
    {
        if (eof_reached_)
        {
            return {-1, empty_line}; // Return -1 and a reference to an empty string if the file is fully read
        }

        load_next_buffer(); // Load the next 10 lines into the buffer
    }

    if (!buffer_.empty())
    {
        const std::string& line = buffer_.front(); // Get a reference to the first line in the buffer
      //  buffer_.erase(buffer_.begin());           // Remove the line from the buffer
        return {0, line};                         // Return 0 and the reference to the line
    }

    return {-1, empty_line}; // Return -1 and a reference to an empty string if no lines are available
}

void LogReader::deleteProcessedLine()
{
    if (!buffer_.empty())
    {
        buffer_.erase(buffer_.begin()); // Remove the first line from the buffer
    }   
}