# GUI test checklist

Record actual outcomes and attach screenshots after testing.

| ID | Test | Steps | Expected result | Actual result |
|---|---|---|---|---|
| G1 | Launch | Run `python3 gui/app.py` | GUI opens and shows process tree | |
| G2 | Process count | Observe top summary | Process count is displayed | |
| G3 | Search by name | Search `bash` or `python` | Matching processes and ancestors appear | |
| G4 | Search by PID | Search a visible PID | The process is shown | |
| G5 | Process details | Select a row, click Process details | Detail dialog appears | |
| G6 | Manual refresh | Click Refresh now | Snapshot updates | |
| G7 | Auto-refresh | Wait at least 3 seconds | Last updated time changes | |
| G8 | New process | In another terminal run `sleep 120 &` | Search/refresh shows the process | |
| G9 | Process exits | Kill only the test sleep process from its terminal | Process disappears after refresh | |
| G10 | Safe operation | Use GUI without sudo | GUI works without changing processes | |

## Demo commands

In a second Ubuntu terminal:

```bash
sleep 120 &
echo $!
```

Search for `sleep` in the GUI. Then stop only the process you started:

```bash
kill <PID_FROM_ECHO>
```

Use the GUI's Refresh now button or wait for auto-refresh.
