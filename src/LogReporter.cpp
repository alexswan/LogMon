#include "LogReporter.h"
#include <sstream>
#include <stdexcept>        



 void LogReporter::flush()
{
    if (buffer_.empty())
        return;

    for (const auto& line : buffer_)
        outfile_ << line << '\n';
    outfile_.flush();
    buffer_.clear();
}