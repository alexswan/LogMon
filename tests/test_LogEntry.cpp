#include <gtest/gtest.h>
#include "../src/LogEntry.h"

TEST(LogEntryTest, FromStringValidInput) {
    // Arrange
    std::string input = "12:34:56,example,START,1234";

    // Act
    LogEntry entry = LogEntry::from_string(input);

    // Assert
    EXPECT_EQ(entry.get_name(), "example");
    EXPECT_EQ(entry.get_pid(), 1234);
    EXPECT_EQ(entry.get_tag(), LogType::START);

    // Check timestamp
    auto timestamp = entry.get_timestamp_sec();

    int hours = timestamp / 3600;
    int minutes = (timestamp % 3600) / 60;
    int seconds = timestamp % 60;

  
    EXPECT_EQ(hours, 12);
    EXPECT_EQ(minutes, 34);
    EXPECT_EQ(seconds, 56);
}

TEST(LogEntryTest, FromStringInvalidInput) {
    // Arrange
    std::string input = "invalid,input";

    // Act
    LogEntry entry = LogEntry::from_string(input);


    // Assert
    EXPECT_EQ(entry.get_name(), "");
    EXPECT_EQ(entry.get_pid(), -1);
    EXPECT_EQ(entry.get_tag(), LogType::START); // Default tag
}

TEST(LogEntryTest, FromStringListValidInputs) {
    // Arrange
    std::vector<std::string> inputs = {
        "1b:34:56,example1,START,1234",// Invalid timestamp
        "01:23:45,あいうえお あいうえお,END,5678",//japanese characters
        "23:59:59,example3,ERR--OR,9999",// Invalid tag
        "26:59:59,example3,ERROR,9999",// Invalid hour
    };

    std::vector<LogEntry> entries;

    // Act
    for (const auto& input : inputs) {
        entries.push_back(LogEntry::from_string(input));
    }

    // Assert
    ASSERT_EQ(entries.size(), 4);

    EXPECT_EQ(entries[0].get_name(), "");
    EXPECT_EQ(entries[0].get_pid(), -1);
    EXPECT_EQ(entries[0].get_tag(), LogType::START);
    EXPECT_EQ(entries[0].get_timestamp_sec(), -1); // 12:34:56 in seconds

    EXPECT_EQ(entries[1].get_name(), "あいうえお あいうえお");
    EXPECT_EQ(entries[1].get_pid(), 5678);
    EXPECT_EQ(entries[1].get_tag(), LogType::END);
    EXPECT_EQ(entries[1].get_timestamp_sec(), 5025); // 01:23:45 in seconds

    EXPECT_EQ(entries[2].get_name(), "");
    EXPECT_EQ(entries[2].get_pid(), -1);
    EXPECT_EQ(entries[2].get_tag(), LogType::START); // Default to START for invalid tag
    EXPECT_EQ(entries[2].get_timestamp_sec(), -1); // 23:59:59 in seconds

   // EXPECT_EQ(entries[3].get_name(), "");
    EXPECT_EQ(entries[3].get_pid(), -1);
    //EXPECT_EQ(entries[3].get_tag(), LogType::START); // Default to START for invalid tag
    EXPECT_EQ(entries[3].get_timestamp_sec(), -1); // 26:59:59 in seconds

}