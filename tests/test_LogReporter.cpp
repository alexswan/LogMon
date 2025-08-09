#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "../src/LogReporter.h"

namespace fs = std::filesystem;

class LogReporterTest : public ::testing::Test {
protected:
    fs::path temp_dir;
    fs::path log_path;

    void SetUp() override {
        temp_dir = fs::temp_directory_path() / fs::path("logwriter_test");
        fs::create_directories(temp_dir);
        log_path = temp_dir / "test.log";
    }

    void TearDown() override {
        fs::remove_all(temp_dir);
    }

    std::vector<std::string> readLines() {
        std::ifstream in(log_path);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(in, line))
            lines.push_back(line);
        return lines;
    }
};

// Writes both warning and error
TEST_F(LogReporterTest, WritesWarningAndError) {
    {
      
        LogReporter log(log_path.string(), LogReporter::Level::Warning, 10);
        log.write(LogReporter::Level::Warning, "hello");
        log.write(LogReporter::Level::Error, "boom");
        log.flush();
    }
    auto lines = readLines();
    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "[WARNING] hello");
    EXPECT_EQ(lines[1], "[ERROR] boom");
}

// Threshold filtering
TEST_F(LogReporterTest, ThresholdFilters) {
    {
        LogReporter log(log_path.string(), LogReporter::Level::Error, 10);
        log.write(LogReporter::Level::Warning, "ignored");
        log.write(LogReporter::Level::Error, "kept");
        log.flush();
    }
    auto lines = readLines();
    ASSERT_EQ(lines.size(), 1);
    EXPECT_EQ(lines[0], "[ERROR] kept");
}

// Auto flush on buffer full
TEST_F(LogReporterTest, AutoFlushOnBufferFull) {
    {
        LogReporter log(log_path.string(), LogReporter::Level::Warning, 2);
        log.write(LogReporter::Level::Warning, "line1");
        log.write(LogReporter::Level::Warning, "line2"); // triggers flush
        log.write(LogReporter::Level::Warning, "line3");
        log.flush();
    }
    auto lines = readLines();
    ASSERT_EQ(lines.size(), 3);
}

// Set level at runtime
TEST_F(LogReporterTest, SetLevelRuntime) {
    {
        LogReporter log(log_path.string(), LogReporter::Level::Warning, 10);
        log.write(LogReporter::Level::Warning, "before");
        log.set_level(LogReporter::Level::Error);
        log.write(LogReporter::Level::Warning, "after"); // ignored
        log.flush();
    }
    auto lines = readLines();
    ASSERT_EQ(lines.size(), 1);
    EXPECT_EQ(lines[0], "[WARNING] before");
}
