# LogMon Monitoring Application


It is a c++ application that reads a log file, analizes the job duration and generates a log based on two threshold intervals.

# What it does?
- it read a file called logs.log that has a structure like this:

11:35:23,scheduled task 032, START,37980
11:35:56,scheduled task 032, END,37980

HH:MM:SS, task name, TAG<START/END>, PID

- it matches same job with START and END entries
- if the job duration is longer than 5 minutes it writes an WARNING in the output log file
- if the job duration is longer than 10 minutes it writes an ERROR in the output file

- at the end, it reports the jobs that are unpaired (missing one of the tags)


# Setup
-Clone the repo:
git clone https://github.com/alexswan/LogMon.git

- it was tested with VSCode and mac
- for the tests: you need to install gtest and update the config.

- run the app from the out folder:
    ./LogMon

- the file it parses is located here:
https://github.com/alexswan/LogMon/blob/main/src/logs.log


- the file generated is logreporter.log as you can see here:
https://github.com/alexswan/LogMon/blob/main/src/logreporter.log

- here is how the output looks like!
![alt screenshot](https://github.com/alexswan/LogMon/blob/main/img/Screenshot%202025-08-09%20at%2001.15.11.png)






