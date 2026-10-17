#pragma once
#include <string>
#include <vector>

struct ProcessInfo {
    int pid = 0;
    int ppid = 0;
    char state = '?';
    std::string name;
    long long memoryKb = 0;
    unsigned long long userTicks = 0;
    unsigned long long systemTicks = 0;
};

std::vector<ProcessInfo> readProcesses();
