# Project Report: Linux Process Tree Visualizer (GUI Edition)

## 1. Problem Definition

Linux runs many concurrent processes. Raw process lists can make parent-child relationships difficult for new users to interpret. This project provides a graphical, hierarchical view of visible processes and their metadata.

## 2. Objectives

- Collect process metadata from Linux `/proc` using C++17.
- Reconstruct parent-child process relationships.
- Display the hierarchy in a desktop GUI.
- Support process search, detail inspection and periodic refresh.
- Provide process-state and resident-memory summaries.
- Operate read-only and safely.

## 3. Technologies

C++17, GNU g++, Linux `/proc`, Python 3, Tkinter, Make.

## 4. Architecture

1. **C++ collector:** reads process stat records and emits tab-separated records.
2. **GUI integration:** Python starts the C++ collector and parses its output.
3. **Hierarchy builder:** links processes to parents and displays branches in a tree widget.
4. **Interaction layer:** search, details dialog, summary labels and timed refresh.

## 5. Implementation

The collector scans numeric directories under `/proc`, reads `/proc/PID/stat`, and extracts the process name, state, PPID, CPU tick counters and resident memory pages. It locates the final `)` in the stat line before parsing later fields because process names can contain spaces and parentheses. RSS pages are converted to KB using the system page size. The GUI uses Tkinter's Treeview widget to display the hierarchy.

## 6. Testing and Results

Run the checklist in `tests/test_cases.md`. Record the date, environment, screenshots and actual results. Do not mark a test passed unless it has been run. Include measured process count from a real run.

## 7. Limitations

The GUI requires WSLg, an X server, or a Linux desktop. Process data is a snapshot refreshed every three seconds. RSS summation may double-count shared memory. CPU tick counters are cumulative and not percentages. Visibility depends on Linux permissions and `/proc` settings.

## 8. Future Enhancements

CPU percentage from consecutive samples, process creation/termination event history, branch filtering, export to CSV and a richer graphical dashboard.

## 9. Responsible Implementation

The program only reads process metadata. It does not send signals, kill processes, change process priorities, or write to `/proc`.
