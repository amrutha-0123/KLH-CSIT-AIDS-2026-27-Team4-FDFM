#include "process_tree.hpp"
#include <iostream>

int main() {
    ProcessTree tree;
    tree.refresh();
    std::cout << "GUI mode is launched with: python3 gui/app.py\n";
    std::cout << "Use the GUI to read process data from /proc.\n";
    return 0;
}
