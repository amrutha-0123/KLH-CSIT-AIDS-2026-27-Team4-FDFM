# Linux Process Tree Visualizer — GUI Edition

A C++17 process collector with a desktop GUI built using Python's Tkinter. The GUI reads real Linux process data from `/proc`, displays the parent-child hierarchy, refreshes automatically, supports searching, and shows process details.

## Requirements

- Ubuntu on WSL 2 (recommended for Windows)
- `g++`, `make`, Python 3 and Tkinter
- Windows 11 with WSLg is recommended for Linux desktop windows. On Windows 10 or WSL without GUI support, use the X server instructions below.

## 1. Install packages in Ubuntu

```bash
sudo apt update
sudo apt install build-essential python3 python3-tk
```

## 2. Extract the ZIP in Windows

Extract `linux-process-tree-visualizer-gui.zip` to a folder, for example:

`C:\Users\YOUR_USERNAME\Downloads\linux-process-tree-visualizer-gui`

## 3. Open that folder in Ubuntu/WSL

Replace `YOUR_USERNAME` with your Windows username:

```bash
cd /mnt/c/Users/YOUR_USERNAME/Downloads/linux-process-tree-visualizer-gui
```

Check that the files are present:

```bash
ls
```

You should see `src`, `include`, `gui`, `Makefile`, and `README.md`.

## 4. Compile the C++ process collector

```bash
make
```

This builds `process-collector`.

## 5. Launch the GUI

```bash
python3 gui/app.py
```

A desktop window should open. The GUI includes:
- Hierarchical process tree
- PID, parent PID, state and resident memory
- Search by process name or PID
- Process details dialog (select a row and click Process details, or double-click)
- Process count, state summary and approximate summed RSS
- Automatic refresh every 3 seconds and a manual Refresh now button

## If the GUI window does not open

### Windows 11 with WSLg
Update WSL from PowerShell:

```powershell
wsl --update
wsl --shutdown
```

Reopen Ubuntu and try again. WSLg is generally available on updated Windows 11 installations.

### If you see `no display name and no $DISPLAY environment variable`
Your WSL setup may not have a Linux GUI display. Options:
1. Use an updated Windows 11 installation with WSLg.
2. On Windows 10 or a setup without WSLg, install and start a Windows X server such as VcXsrv, then configure `DISPLAY` in Ubuntu according to that X server's instructions.
3. Run the project on a native Ubuntu desktop or college Linux computer.

Do not run the GUI with `sudo`; ordinary user access is sufficient.

## Testing

1. Launch the app and confirm a process tree appears.
2. Search for `bash`, `python`, or another process name present on your system.
3. Select a process and open Process details.
4. Wait 3 seconds and check the Last updated time.
5. In a second Ubuntu terminal, run `sleep 120 &`, note the PID, and search for `sleep`.
6. Stop only that test process with `kill <PID>` from the second terminal. Observe the refreshed tree.

Record actual test outcomes and screenshots for your report.

## Architecture

- `src/process_tree.cpp`: C++ reads and parses `/proc/PID/stat`.
- `src/collector.cpp`: emits process records in a tab-separated format.
- `gui/app.py`: Tkinter desktop UI, process hierarchy, search, statistics and refresh timer.
- `include/process_tree.hpp`: shared process data structure and collector interface.

## Safety and limitations

This application is read-only. It does not stop, kill, reprioritize or modify processes. Process entries can disappear during a scan. The memory summary adds RSS values and can double-count shared memory. CPU tick values are cumulative counters; this version does not calculate CPU percentage. GUI display requires WSLg, an X server, or a Linux desktop.
