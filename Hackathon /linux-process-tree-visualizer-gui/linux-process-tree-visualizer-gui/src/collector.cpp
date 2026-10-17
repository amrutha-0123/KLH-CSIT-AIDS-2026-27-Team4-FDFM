#include "process_tree.hpp"
#include <iostream>
#include <string>
#include <vector>

// The GUI uses tab-separated output from this small C++ collector.
// Process names containing tabs/newlines are unusual; replace tabs for safe output.
int main() {
    for (const auto& p : readProcesses()) {
        std::string safeName = p.name;
        for (char& c : safeName) if (c == '\t' || c == '\n' || c == '\r') c = ' ';
        std::cout << p.pid << '\t' << p.ppid << '\t' << p.state << '\t'
                  << p.memoryKb << '\t' << p.userTicks << '\t'
                  << p.systemTicks << '\t' << safeName << '\n';
    }
    return 0;
}
