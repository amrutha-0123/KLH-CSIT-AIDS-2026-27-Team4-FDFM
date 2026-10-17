#!/usr/bin/env python3
"""Tkinter GUI for the C++ Linux Process Tree Visualizer.

The GUI reads real process metadata from Linux /proc. It uses a small C++ helper
for collection and can also collect via Python if the helper has not been built.
"""
import os
import subprocess
import time
import tkinter as tk
from tkinter import ttk, messagebox
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HELPER = ROOT / "process-collector"

def collect():
    """Call the C++ helper; parse its tab-separated output."""
    try:
        completed = subprocess.run(
            [str(HELPER)], capture_output=True, text=True, timeout=5, check=True
        )
        rows = []
        for line in completed.stdout.splitlines():
            parts = line.split("\t")
            if len(parts) >= 7:
                try:
                    rows.append({
                        "pid": int(parts[0]), "ppid": int(parts[1]),
                        "state": parts[2], "memory": int(parts[3]),
                        "utime": int(parts[4]), "stime": int(parts[5]),
                        "name": parts[6]
                    })
                except ValueError:
                    pass
        if rows:
            return rows
    except (OSError, subprocess.SubprocessError):
        pass
    # Fallback collector keeps the GUI usable if the C++ helper isn't built.
    import glob
    rows = []
    for path in glob.glob("/proc/[0-9]*/stat"):
        try:
            line = open(path, encoding="utf-8", errors="replace").read().strip()
            pid = int(path.split("/")[-2])
            left, right = line.find("("), line.rfind(")")
            if left < 0 or right < left:
                continue
            name = line[left + 1:right]
            f = line[right + 1:].split()
            if len(f) <= 21:
                continue
            pages = int(f[21])
            rows.append({"pid": pid, "ppid": int(f[1]), "state": f[0],
                         "memory": max(0, pages * os.sysconf("SC_PAGE_SIZE") // 1024),
                         "utime": int(f[11]), "stime": int(f[12]), "name": name})
        except (OSError, ValueError, IndexError):
            continue
    return sorted(rows, key=lambda x: x["pid"])

class ProcessVisualizer:
    def __init__(self, root):
        self.root = root
        self.root.title("Linux Process Tree Visualizer")
        self.root.geometry("1120x720")
        self.root.minsize(850, 540)
        self.processes = []
        self.by_pid = {}
        self.after_id = None
        self.auto_refresh = tk.BooleanVar(value=True)
        self.search_text = tk.StringVar()
        self.status_text = tk.StringVar(value="Ready")
        self._style()
        self._build()
        self.refresh()

    def _style(self):
        style = ttk.Style()
        try:
            style.theme_use("clam")
        except tk.TclError:
            pass
        style.configure("Treeview", rowheight=27, font=("Segoe UI", 10))
        style.configure("Treeview.Heading", font=("Segoe UI", 10, "bold"))
        style.configure("Title.TLabel", font=("Segoe UI", 18, "bold"))
        style.configure("Stat.TLabel", font=("Segoe UI", 11, "bold"))

    def _build(self):
        outer = ttk.Frame(self.root, padding=16)
        outer.pack(fill="both", expand=True)

        header = ttk.Frame(outer)
        header.pack(fill="x", pady=(0, 12))
        ttk.Label(header, text="Linux Process Tree Visualizer", style="Title.TLabel").pack(side="left")
        ttk.Label(header, text="C++ / Linux /proc", foreground="#64748b").pack(side="right", pady=7)

        stats = ttk.Frame(outer)
        stats.pack(fill="x", pady=(0, 12))
        self.total_label = ttk.Label(stats, text="Processes: —", style="Stat.TLabel")
        self.total_label.pack(side="left", padx=(0, 24))
        self.memory_label = ttk.Label(stats, text="Summed RSS: —", style="Stat.TLabel")
        self.memory_label.pack(side="left", padx=(0, 24))
        self.state_label = ttk.Label(stats, text="States: —")
        self.state_label.pack(side="left")

        controls = ttk.Frame(outer)
        controls.pack(fill="x", pady=(0, 10))
        ttk.Label(controls, text="Search name or PID:").pack(side="left")
        search = ttk.Entry(controls, textvariable=self.search_text, width=28)
        search.pack(side="left", padx=8)
        search.bind("<KeyRelease>", lambda _e: self.apply_filter())
        ttk.Button(controls, text="Refresh now", command=self.refresh).pack(side="left", padx=(0, 8))
        ttk.Checkbutton(controls, text="Auto-refresh every 3 seconds",
                        variable=self.auto_refresh, command=self.schedule_refresh).pack(side="left")
        ttk.Button(controls, text="Process details", command=self.show_details).pack(side="right")

        columns = ("pid", "ppid", "state", "memory", "name")
        self.tree = ttk.Treeview(outer, columns=columns, show="tree headings", selectmode="browse")
        self.tree.heading("#0", text="Process hierarchy")
        self.tree.column("#0", width=430, minwidth=250)
        for col, title, width in [
            ("pid", "PID", 85), ("ppid", "Parent PID", 100),
            ("state", "State", 70), ("memory", "RSS (KB)", 110), ("name", "Process name", 220)
        ]:
            self.tree.heading(col, text=title)
            self.tree.column(col, width=width, anchor="w")
        self.tree.pack(fill="both", expand=True)
        self.tree.bind("<Double-1>", lambda _e: self.show_details())

        bottom = ttk.Frame(outer)
        bottom.pack(fill="x", pady=(10, 0))
        ttk.Label(bottom, textvariable=self.status_text).pack(side="left")
        ttk.Label(bottom, text="Read-only: no processes are stopped or modified",
                  foreground="#15803d").pack(side="right")

    def refresh(self):
        try:
            self.processes = collect()
            self.by_pid = {p["pid"]: p for p in self.processes}
            self.total_label.config(text=f"Processes: {len(self.processes)}")
            total_mb = sum(p["memory"] for p in self.processes) / 1024
            self.memory_label.config(text=f"Summed RSS: {total_mb:,.1f} MB")
            states = {}
            for p in self.processes:
                states[p["state"]] = states.get(p["state"], 0) + 1
            self.state_label.config(text="States: " + "  ".join(f"{k}:{v}" for k, v in sorted(states.items())))
            self.apply_filter()
            self.status_text.set(f"Last updated: {time.strftime('%H:%M:%S')}  •  {len(self.processes)} processes read")
        except Exception as exc:
            self.status_text.set(f"Refresh error: {exc}")
        self.schedule_refresh()

    def apply_filter(self):
        query = self.search_text.get().strip().lower()
        for item in self.tree.get_children(""):
            self.tree.delete(item)
        visible = {}
        for p in self.processes:
            if not query or query in p["name"].lower() or query == str(p["pid"]):
                visible[p["pid"]] = p
        if query:
            # Search view shows matches plus their ancestors where possible.
            for pid in list(visible):
                current = visible[pid]
                hops = 0
                while current["ppid"] in self.by_pid and hops < 100:
                    parent = self.by_pid[current["ppid"]]
                    visible[parent["pid"]] = parent
                    current = parent
                    hops += 1
        children = {}
        for pid, p in visible.items():
            parent = p["ppid"]
            parent_key = parent if parent in visible and parent != pid else 0
            children.setdefault(parent_key, []).append(pid)
        for kids in children.values():
            kids.sort()
        inserted = set()

        def add_branch(pid, parent_item="", visited=None):
            if visited is None:
                visited = set()
            if pid in visited or pid not in visible:
                return
            visited.add(pid)
            p = visible[pid]
            label = f"{p['name']}  (PID {pid})"
            item = self.tree.insert(parent_item, "end", text=label,
                                    values=(pid, p["ppid"], p["state"], p["memory"], p["name"]),
                                    open=True)
            inserted.add(pid)
            for child in children.get(pid, []):
                add_branch(child, item, visited)

        for pid in children.get(0, []):
            add_branch(pid)
        # Avoid losing processes with invisible/missing parents or odd /proc snapshots.
        for pid in sorted(visible):
            if pid not in inserted:
                add_branch(pid)

    def show_details(self):
        selected = self.tree.selection()
        if not selected:
            messagebox.showinfo("Select a process", "Select a process in the tree first.")
            return
        values = self.tree.item(selected[0], "values")
        try:
            pid = int(values[0])
            p = self.by_pid[pid]
        except (ValueError, KeyError, IndexError):
            return
        messagebox.showinfo(
            f"Process details — PID {pid}",
            f"Name: {p['name']}\nPID: {p['pid']}\nParent PID: {p['ppid']}\n"
            f"State: {p['state']}\nResident memory: {p['memory']:,} KB\n"
            f"User CPU ticks: {p['utime']}\nSystem CPU ticks: {p['stime']}\n\n"
            "CPU ticks are cumulative counters, not CPU percentages."
        )

    def schedule_refresh(self):
        if self.after_id:
            try:
                self.root.after_cancel(self.after_id)
            except tk.TclError:
                pass
            self.after_id = None
        if self.auto_refresh.get():
            self.after_id = self.root.after(3000, self.refresh)

def main():
    if not os.path.exists("/proc"):
        print("This application must run in Ubuntu/Linux/WSL, not native Windows Python.")
        raise SystemExit(1)
    root = tk.Tk()
    ProcessVisualizer(root)
    root.mainloop()

if __name__ == "__main__":
    main()
