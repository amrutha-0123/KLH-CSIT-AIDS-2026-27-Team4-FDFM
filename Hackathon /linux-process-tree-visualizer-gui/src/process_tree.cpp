#include "process_tree.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace fs = std::filesystem;

static bool readOne(int pid, ProcessInfo& p) {
    std::ifstream file("/proc/" + std::to_string(pid) + "/stat");
    std::string line;
    if (!file || !std::getline(file, line)) return false;
    auto open = line.find('(');
    auto close = line.rfind(')');
    if (open == std::string::npos || close == std::string::npos || close <= open) return false;
    p.pid = pid;
    p.name = line.substr(open + 1, close - open - 1);
    std::istringstream in(line.substr(close + 1));
    std::vector<std::string> v;
    std::string token;
    while (in >> token) v.push_back(token);
    if (v.size() <= 21) return false;
    try {
        p.state = v[0].empty() ? '?' : v[0][0];
        p.ppid = std::stoi(v[1]);
        p.userTicks = std::stoull(v[11]);
        p.systemTicks = std::stoull(v[12]);
        long long pages = std::stoll(v[21]);
        long pageSize = sysconf(_SC_PAGESIZE);
        p.memoryKb = pages > 0 && pageSize > 0 ? pages * pageSize / 1024 : 0;
    } catch (...) { return false; }
    return true;
}

std::vector<ProcessInfo> readProcesses() {
    std::vector<ProcessInfo> result;
    std::error_code ec;
    for (fs::directory_iterator it("/proc", fs::directory_options::skip_permission_denied, ec), end;
         it != end; it.increment(ec)) {
        if (ec) { ec.clear(); continue; }
        auto name = it->path().filename().string();
        if (name.empty() || !std::all_of(name.begin(), name.end(),
            [](unsigned char c) { return std::isdigit(c); })) continue;
        try {
            ProcessInfo p;
            if (readOne(std::stoi(name), p)) result.push_back(p);
        } catch (...) {}
    }
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) { return a.pid < b.pid; });
    return result;
}
