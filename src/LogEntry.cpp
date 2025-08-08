#include "LogEntry.h"

LogEntry::LogEntry(int timestamp, std::string name, LogType tag, int pid) noexcept
    : timestamp_(timestamp), name_(std::move(name)), tag_(tag), pid_(pid) {};


LogEntry LogEntry::from_string(const std::string& line) noexcept
    {
        LogEntry entry;

        if (line.empty())
        {
            return entry; // return an empty LogEntry if the line is empty
        }

        
        int timestamp = -1;
        std::string name;
        LogType tag = LogType::START; // default tag
        int pid = -1;

        
        size_t start = 0;
           
        size_t end = line.size();

        int tokenindex = 0;

        while (start < end)
        {
            size_t pos = line.find(',', start);

            if (pos == std::string::npos)
            {
                pos = end; // if no space found, take the rest of the line
            }

            std::string token = line.substr(start, pos - start);
            
            switch(tokenindex)
            {
                case 0:
                    {
                        timestamp = LogEntry::parse_timestamp(token); // first token is DateTime
                        if (timestamp == -1) 
                        {
                            return entry; // return an empty LogEntry if timestamp parsing fails
                        }
                    }
                    break;
                case 1:
                    {
                        name = token; // second token is name
                    }
                    break;
                case 2: 
                    {
                        int tagpos = token.find("START");
                        if (tagpos != std::string::npos) 
                        {
                            tag = LogType::START;
                        }
                        else if (token.find("END") != std::string::npos)
                        {
                            tag = LogType::END;
                        }
                        else
                        {
                            tag = LogType::START; // default to START if tag is invalid
                            return entry; 
                        }
                    }break;
                case 3:
                    {
                        try {
                            pid = std::stoi(token); // fourth token is pid
                        } catch (const std::invalid_argument&) {
                            pid = -1; // set pid to -1 if conversion fails
                        } catch (const std::out_of_range&) {
                            pid = -1; // set pid to -1 if conversion fails
                        }
                    }break;
                default:
                    return entry; // return an empty LogEntry if there are more than 4 tokens
            }
                
            tokenindex++;

            start = pos + 1;        // move to the next token
        }

         

        return LogEntry(timestamp, std::move(name), tag, pid);
    }


    int LogEntry::parse_timestamp(const std::string& datetime) noexcept 
    {
        struct tm tm = {};
        if(strptime(datetime.c_str(), "%H:%M:%S", &tm))
        {
            // Calculate the timestamp as seconds
            return tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
        }
        else
        {
            //std::cerr << "Failed to parse timestamp: " << datetime << std::endl;
            return -1; // Return an invalid timestamp if parsing fails
        }
    }


    std::string LogEntry::timestamp_to_time(int timestamp) noexcept 
    {
        int hours = timestamp / 3600;
        int minutes = (timestamp % 3600) / 60;
        int seconds = timestamp % 60;

        // Format the time as HH:MM:SS
        char buffer[9];
        snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
        return std::string(buffer);
   }