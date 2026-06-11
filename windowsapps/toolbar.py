"""
Dual Terminal Shortcut Trigger
Two rows of 6 configurable buttons, each row targets a separate window.

Requirements:
    pip install pywin32 pyautogui

Usage:
    - Click "Pick Target" on a row, then click any open window to target it
    - Each button can send a text command (types it + Enter) or a key combo
    - Right-click any button to configure it
    - Settings saved to toolbar_config.json next to this script
"""

import sys
import os
import json
import time
import threading
import tkinter as tk
from tkinter import simpledialog, messagebox

try:
    import win32gui
    import win32con
    import win32api
    HAS_WIN32 = True
except ImportError:
    HAS_WIN32 = False

try:
    import pyautogui
    HAS_PYAUTOGUI = True
except ImportError:
    HAS_PYAUTOGUI = False

# ── Config ────────────────────────────────────────────────────────────────────

if getattr(sys, 'frozen', False):
    BASE_DIR = os.path.dirname(sys.executable)
else:
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))

CONFIG_FILE = os.path.join(BASE_DIR, "toolbar_config.json")

DEFAULT_ROW = lambda label: {
    "target_hwnd": None,
    "target_title": "",
    "buttons": [
        {"label": label + " 1", "type": "cmd", "value": ""},
        {"label": label + " 2", "type": "cmd", "value": ""},
        {"label": label + " 3", "type": "cmd", "value": ""},
        {"label": label + " 4", "type": "cmd", "value": ""},
        {"label": label + " 5", "type": "cmd", "value": ""},
        {"label": label + " 6", "type": "cmd", "value": ""},
    ]
}

DEFAULT_CONFIG = {
    "row1": DEFAULT_ROW("Row 1"),
    "row2": DEFAULT_ROW("Row 2"),
    "window_x": 100,
    "window_y": 100,
}

# Pre-fill row1 with ESP-IDF build commands, row2 with git commands
DEFAULT_CONFIG["row1"]["buttons"] = [
    {"label": "Preflight",  "type": "cmd", "value": ".\\preflight_check.bat",                                                                                                                "alt": 1},
    {"label": "Clean",      "type": "cmd", "value": "idf.py fullclean 2>&1 | Out-Null",                                                                                                      "alt": 2},
    {"label": "Build",      "type": "cmd", "value": "idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath \"claudetransferv2\\build_log.txt\" -Encoding UTF8",      "alt": 3},
    {"label": "Flash",      "type": "cmd", "value": "idf.py -p COM4 flash",                                                                                                                  "alt": 4},
    {"label": "Parse",      "type": "cmd", "value": ".\\parse_logs.ps1 claudetransferv2\\build_log.txt build",                                                                               "alt": 5},
    {"label": "Monitor",    "type": "cmd", "value": "idf.py -p COM4 monitor --no-reset 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath \"claudetransferv2\\monitor_log.txt\" -Encoding UTF8", "alt": 6},
]
DEFAULT_CONFIG["row2"]["buttons"] = [
    {"label": "Git Status", "type": "cmd", "value": "git status",              "alt": 7},
    {"label": "Git Add",    "type": "cmd", "value": "git add -A",              "alt": 8},
    {"label": "Git Log",    "type": "cmd", "value": "git log --oneline -10",   "alt": 9},
    {"label": "Git Push",   "type": "cmd", "value": "git push",                "alt": 10},
    {"label": "Dir",        "type": "cmd", "value": "dir",                     "alt": 11},
    {"label": "CLS",        "type": "key", "value": "ctrl+l",                  "alt": 12},
]


def load_config():
    if os.path.isfile(CONFIG_FILE):
        try:
            with open(CONFIG_FILE, "r") as f:
                data = json.load(f)
            for k, v in DEFAULT_CONFIG.items():
                if k not in data:
                    data[k] = v
            return data
        except Exception:
            pass
    return {k: (v.copy() if isinstance(v, dict) else v)
            for k, v in DEFAULT_CONFIG.items()}


def save_config(cfg):
    with open(CONFIG_FILE, "w") as f:
        json.dump(cfg, f, indent=2)


# ── Window picker ─────────────────────────────────────────────────────────────

class WindowPicker:
    """
    Makes the cursor a crosshair; on click returns the HWND under the cursor.
    Uses a fullscreen transparent overlay to intercept the click.
    """
    def __init__(self, root, callback):
        self.root = root
        self.callback = callback
        self._build_overlay()

    def _build_overlay(self):
        self.overlay = tk.Toplevel(self.root)
        self.overlay.attributes("-fullscreen", True)
        self.overlay.attributes("-alpha", 0.01)
        self.overlay.attributes("-topmost", True)
        self.overlay.config(cursor="crosshair", bg="black")
        self.overlay.bind("<Button-1>", self._on_click)
        self.overlay.bind("<Escape>", lambda e: self._cancel())
        self.overlay.focus_force()

    def _on_click(self, event):
        self.overlay.destroy()
        # Small delay so the overlay is gone before we read the hwnd
        self.root.after(100, self._get_hwnd)

    def _get_hwnd(self):
        if HAS_WIN32:
            x, y = win32api.GetCursorPos()
            hwnd = win32gui.WindowFromPoint((x, y))
            # Walk up to find a top-level window
            parent = win32gui.GetParent(hwnd)
            while parent:
                hwnd = parent
                parent = win32gui.GetParent(hwnd)
            title = win32gui.GetWindowText(hwnd)
            self.callback(hwnd, title)
        else:
            self.callback(None, "")

    def _cancel(self):
        self.overlay.destroy()
        self.callback(None, None)


# ── Button config dialog ──────────────────────────────────────────────────────

class ButtonConfigDialog(tk.Toplevel):
    def __init__(self, parent, bdef, on_save):
        super().__init__(parent)
        self.bdef = bdef
        self.on_save = on_save
        self.title("Configure Button")
        self.resizable(False, False)
        self.configure(bg="#1e1e1e")
        self.grab_set()

        BG = "#1e1e1e"
        FG = "#e0e0e0"
        ENTRY_BG = "#2d2d2d"
        ACC = "#E87B2C"
        FONT = ("Segoe UI", 9)
        FONT_B = ("Segoe UI", 9, "bold")

        tk.Label(self, text="Label:", bg=BG, fg=FG, font=FONT).grid(
            row=0, column=0, padx=10, pady=6, sticky="w")
        self.label_var = tk.StringVar(value=bdef.get("label", ""))
        tk.Entry(self, textvariable=self.label_var, width=30,
                 bg=ENTRY_BG, fg=FG, insertbackground=FG,
                 relief="flat", font=FONT).grid(row=0, column=1, padx=10, pady=6)

        tk.Label(self, text="Type:", bg=BG, fg=FG, font=FONT).grid(
            row=1, column=0, padx=10, pady=4, sticky="w")
        self.type_var = tk.StringVar(value=bdef.get("type", "cmd"))
        type_frame = tk.Frame(self, bg=BG)
        type_frame.grid(row=1, column=1, padx=10, sticky="w")
        tk.Radiobutton(type_frame, text="Command (types + Enter)", variable=self.type_var,
                       value="cmd", bg=BG, fg=FG, selectcolor="#3a3a3a",
                       activebackground=BG, font=FONT).pack(side="left")
        tk.Radiobutton(type_frame, text="Key combo", variable=self.type_var,
                       value="key", bg=BG, fg=FG, selectcolor="#3a3a3a",
                       activebackground=BG, font=FONT).pack(side="left", padx=8)

        tk.Label(self, text="Value:", bg=BG, fg=FG, font=FONT).grid(
            row=2, column=0, padx=10, pady=4, sticky="w")
        self.value_var = tk.StringVar(value=bdef.get("value", ""))
        tk.Entry(self, textvariable=self.value_var, width=40,
                 bg=ENTRY_BG, fg=FG, insertbackground=FG,
                 relief="flat", font=("Consolas", 9)).grid(row=2, column=1, padx=10, pady=4)

        tk.Label(self, text="Key combo examples: ctrl+c, alt+f4, f5",
                 bg=BG, fg="#666666", font=("Segoe UI", 8)).grid(
            row=3, column=1, padx=10, sticky="w")

        btn_row = tk.Frame(self, bg=BG)
        btn_row.grid(row=4, column=0, columnspan=2, pady=12)
        tk.Button(btn_row, text="Save", bg=ACC, fg="white", relief="flat",
                  font=FONT_B, width=10, cursor="hand2",
                  command=self._save).pack(side="left", padx=6)
        tk.Button(btn_row, text="Cancel", bg="#333333", fg=FG, relief="flat",
                  font=FONT, width=8, cursor="hand2",
                  command=self.destroy).pack(side="left", padx=6)

    def _save(self):
        self.bdef["label"] = self.label_var.get()
        self.bdef["type"]  = self.type_var.get()
        self.bdef["value"] = self.value_var.get()
        self.on_save(self.bdef)
        self.destroy()


# ── Toolbar row ───────────────────────────────────────────────────────────────

class ToolbarRow(tk.Frame):
    def __init__(self, parent, row_key, cfg, app, color, **kwargs):
        super().__init__(parent, **kwargs)
        self.row_key = row_key
        self.cfg = cfg
        self.app = app
        self.color = color
        self.target_hwnd = cfg[row_key].get("target_hwnd")
        self.target_title = cfg[row_key].get("target_title", "")

        BG     = "#1a1a1a"
        FG     = "#e0e0e0"
        BTN_BG = "#2a2a2a"
        FONT   = ("Segoe UI", 8)
        FONT_B = ("Segoe UI", 8, "bold")

        self.configure(bg=BG)

        # Target indicator + pick button
        left = tk.Frame(self, bg=BG)
        left.pack(side="left", padx=(6, 4))

        self.target_lbl = tk.Label(left, text=self._target_text(),
                                   bg=BG, fg="#888888",
                                   font=("Segoe UI", 7),
                                   width=16, anchor="w")
        self.target_lbl.pack(anchor="w")

        tk.Button(left, text="Pick Target", bg=color, fg="white",
                  relief="flat", font=FONT_B, cursor="hand2",
                  width=10, activebackground=color,
                  command=self._pick_target).pack()

        # Divider
        tk.Frame(self, bg="#333333", width=1).pack(side="left", fill="y", padx=4)

        # 6 buttons
        self.btns = []
        for i, bdef in enumerate(cfg[row_key]["buttons"]):
            b = tk.Button(self, text=bdef["label"],
                          bg=BTN_BG, fg=FG, relief="flat",
                          font=FONT, cursor="hand2", width=11,
                          activebackground="#3a3a3a",
                          command=lambda d=bdef: self._fire(d))
            b.pack(side="left", padx=2, pady=4)
            b.bind("<Button-3>", lambda e, idx=i: self._right_click(idx))
            self.btns.append(b)

    def _target_text(self):
        t = self.target_title
        if not t:
            return "No target"
        return t[:20] + "…" if len(t) > 20 else t

    def _pick_target(self):
        self.app.title("Click the window to target…")
        WindowPicker(self.app, self._on_target_picked)

    def _on_target_picked(self, hwnd, title):
        self.app.title("Dual Terminal Shortcut Trigger")
        if title is None:
            return  # cancelled
        if hwnd:
            self.target_hwnd = hwnd
            self.target_title = title
            self.cfg[self.row_key]["target_hwnd"] = hwnd
            self.cfg[self.row_key]["target_title"] = title
            save_config(self.cfg)
            self.target_lbl.config(text=self._target_text())
        else:
            messagebox.showerror("pywin32 missing",
                                 "Install pywin32:\n  pip install pywin32")

    def _fire(self, bdef):
        val = bdef.get("value", "").strip()
        if not val:
            return
        if not self.target_hwnd:
            messagebox.showinfo("No target", "Pick a target window first.")
            return

        toolbar_hwnd = self.app.winfo_id() if HAS_WIN32 else None

        def _send():
            try:
                if HAS_WIN32:
                    win32gui.ShowWindow(self.target_hwnd, win32con.SW_RESTORE)
                    win32gui.SetForegroundWindow(self.target_hwnd)
                    time.sleep(0.15)

                if HAS_PYAUTOGUI:
                    # Auto-detect if value is a key combo or a text command
                    # Key combo if: starts with < (tkinter seq), or contains +
                    # with no spaces (e.g. ctrl+g, shift+f5), or is a single
                    # word that matches a known key name (f1-f12, tab, etc)
                    is_key = False
                    if val.startswith("<") and val.endswith(">"):
                        is_key = True
                    elif "+" in val and " " not in val:
                        is_key = True
                    elif val.lower() in (
                        "f1","f2","f3","f4","f5","f6","f7","f8","f9",
                        "f10","f11","f12","tab","enter","escape","space",
                        "backspace","delete","home","end","pageup","pagedown",
                        "up","down","left","right","insert","printscreen"):
                        is_key = True

                    if is_key:
                        if val.startswith("<") and val.endswith(">"):
                            keys = sequence_to_pyautogui(val)
                        else:
                            keys = [k.strip().lower() for k in val.split("+")]
                        if len(keys) == 1:
                            pyautogui.press(keys[0])
                        else:
                            pyautogui.hotkey(*keys)
                    else:
                        # Text command — type it and hit Enter
                        pyautogui.typewrite(val, interval=0.03)
                        pyautogui.press("enter")
                else:
                    self.after(0, lambda: messagebox.showerror("pyautogui missing",
                                         "Install pyautogui:\n  pip install pyautogui"))

                # Wait for command to be received then steal focus back to toolbar
                time.sleep(0.3)
                if HAS_WIN32 and toolbar_hwnd:
                    try:
                        # Get the actual top-level toolbar window handle
                        tw = win32gui.GetAncestor(toolbar_hwnd, 2)  # GA_ROOT = 2
                        win32gui.ShowWindow(tw, win32con.SW_SHOW)
                        win32gui.SetForegroundWindow(tw)
                    except Exception:
                        pass

            except Exception as ex:
                self.after(0, lambda: messagebox.showerror("Send error", str(ex)))

        threading.Thread(target=_send, daemon=True).start()

    def _right_click(self, idx):
        bdef = self.cfg[self.row_key]["buttons"][idx]
        def on_save(updated):
            self.btns[idx].config(text=updated["label"])
            save_config(self.cfg)
        ButtonConfigDialog(self.app, bdef, on_save)


# ── Shortcut key capture ──────────────────────────────────────────────────────

def event_to_sequence(event):
    """Convert a tkinter key event to a bind sequence string.
    On this machine base state=0x8 (NumLock). Ctrl=0x4, Shift=0x1.
    """
    key = event.keysym
    if not key or key in ("Control_L","Control_R","Alt_L","Alt_R",
                           "Shift_L","Shift_R","Super_L","Super_R",
                           "Menu", "Win_L", "Win_R"):
        return None
    mods = ""
    if event.state & 0x4: mods += "Control-"
    if event.state & 0x1: mods += "Shift-"
    # Use lowercase key for consistency except function keys
    if not key.startswith("F") or not key[1:].isdigit():
        key = key.lower()
    return f"<{mods}{key}>"

def sequence_to_pyautogui(seq):
    """Convert tkinter bind sequence to pyautogui hotkey args list.
    e.g. '<Control-g>' -> ['ctrl', 'g']
         '<Shift-F5>'  -> ['shift', 'f5']
         '<F5>'        -> ['f5']
         '<Control-F5>' -> ['ctrl', 'f5']
    """
    if not seq:
        return []
    inner = seq.strip("<>")
    # Split on - but keep function keys intact (F5, F12 etc)
    parts = inner.split("-")
    result = []
    for p in parts:
        pl = p.lower()
        if pl == "control":   result.append("ctrl")
        elif pl == "shift":   result.append("shift")
        elif pl == "alt":     result.append("alt")
        elif p.startswith("F") and p[1:].isdigit(): result.append(pl)  # f5, f12
        else:                 result.append(pl)
    return result

def sequence_display(seq):
    """Human readable version of a bind sequence."""
    if not seq:
        return ""
    return seq.strip("<>").replace("-", "+")


# ── Shortcut preferences dialog ──────────────────────────────────────────────

class ShortcutPrefsDialog(tk.Toplevel):
    def __init__(self, parent, cfg, on_save):
        super().__init__(parent)
        self.cfg = cfg
        self.on_save = on_save
        self.title("Preferences — Shortcut Keys")
        self.resizable(False, False)
        self.configure(bg="#1e1e1e")
        self.grab_set()

        BG       = "#1e1e1e"
        FG       = "#e0e0e0"
        ENTRY_BG = "#2d2d2d"
        ACC      = "#E87B2C"
        FONT     = ("Segoe UI", 9)
        FONT_B   = ("Segoe UI", 9, "bold")
        DIM      = "#888888"

        tk.Label(self, text="Click [Record] then press any key or combo to capture it.",
                 bg=BG, fg=DIM, font=("Segoe UI", 8)).pack(padx=12, pady=(8,4), anchor="w")

        # Headers
        hdr = tk.Frame(self, bg=BG, padx=12)
        hdr.pack(fill="x", pady=(4, 2))
        for col, w in [("Row", 4), ("Btn", 4), ("Label", 14), ("Command / Key", 26), ("Shortcut", 14), ("", 8)]:
            tk.Label(hdr, text=col, bg=BG, fg=DIM,
                     font=FONT, width=w, anchor="w").pack(side="left", padx=4)

        self.rows = []
        self._capturing = None  # (row_key, idx) currently being captured

        for row_key, row_label in [("row1", "Row 1"), ("row2", "Row 2")]:
            for i, bdef in enumerate(cfg[row_key]["buttons"]):
                row_frame = tk.Frame(self, bg=BG, padx=12, pady=2)
                row_frame.pack(fill="x")

                tk.Label(row_frame, text=row_label, bg=BG, fg=FG,
                         font=FONT, width=4, anchor="w").pack(side="left", padx=4)
                tk.Label(row_frame, text=f"{i+1}", bg=BG, fg=DIM,
                         font=FONT, width=4, anchor="w").pack(side="left", padx=4)

                lv = tk.StringVar(value=bdef.get("label", ""))
                tk.Entry(row_frame, textvariable=lv, width=14,
                         bg=ENTRY_BG, fg=FG, insertbackground=FG,
                         relief="flat", font=FONT).pack(side="left", padx=4)

                vv = tk.StringVar(value=bdef.get("value", ""))
                tk.Entry(row_frame, textvariable=vv, width=26,
                         bg=ENTRY_BG, fg=FG, insertbackground=FG,
                         relief="flat", font=("Consolas", 8)).pack(side="left", padx=4)

                # Shortcut display label
                current_seq = bdef.get("shortcut", "")
                sv = tk.StringVar(value=sequence_display(current_seq))
                seq_lbl = tk.Label(row_frame, textvariable=sv, bg=ENTRY_BG, fg=ACC,
                                   font=("Consolas", 8), width=14, anchor="w",
                                   relief="flat", padx=4)
                seq_lbl.pack(side="left", padx=4)

                # Record button
                rec_btn = tk.Button(row_frame, text="Record", bg="#2a2a2a", fg=FG,
                                    relief="flat", font=FONT, width=7, cursor="hand2")
                rec_btn.pack(side="left", padx=2)

                # Wire up capture
                seq_store = [current_seq]  # mutable container
                def make_capture(rk, idx, sv_, sl_, sb_, ss_):
                    def start_capture():
                        self._capturing = (rk, idx)
                        self.master._capturing_active = True
                        sl_.config(fg="#ffaa00")
                        sv_.set("Press a key...")
                        sb_.config(bg=ACC, fg="white")
                        self.bind("<KeyPress>", lambda e: capture_key(e, rk, idx, sv_, sl_, sb_, ss_))
                    def capture_key(e, rk_, idx_, sv__, sl__, sb__, ss__):
                        seq = event_to_sequence(e)
                        if seq:
                            ss__[0] = seq
                            sv__.set(sequence_display(seq))
                            sl__.config(fg=ACC)
                            sb__.config(bg="#2a2a2a", fg=FG)
                            self.unbind("<KeyPress>")
                            self._capturing = None
                            self.master._capturing_active = False
                        return "break"
                    return start_capture
                rec_btn.config(command=make_capture(row_key, i, sv, seq_lbl, rec_btn, seq_store))

                # Clear button
                def make_clear(sv_, sl_, ss_):
                    def clear():
                        ss_[0] = ""
                        sv_.set("")
                        sl_.config(fg="#888888")
                    return clear
                tk.Button(row_frame, text="✕", bg="#2a2a2a", fg="#888888",
                          relief="flat", font=FONT, width=2, cursor="hand2",
                          command=make_clear(sv, seq_lbl, seq_store)).pack(side="left", padx=(0,4))

                self.rows.append((row_key, i, lv, vv, seq_store))

        btn_row = tk.Frame(self, bg=BG, pady=10)
        btn_row.pack()
        tk.Button(btn_row, text="Save", bg=ACC, fg="white", relief="flat",
                  font=FONT_B, width=10, cursor="hand2",
                  command=self._save).pack(side="left", padx=6)
        tk.Button(btn_row, text="Cancel", bg="#333333", fg=FG, relief="flat",
                  font=FONT, width=8, cursor="hand2",
                  command=self.destroy).pack(side="left", padx=6)

    def _save(self):
        for row_key, idx, lv, vv, seq_store in self.rows:
            self.cfg[row_key]["buttons"][idx]["label"]    = lv.get()
            self.cfg[row_key]["buttons"][idx]["value"]    = vv.get()
            self.cfg[row_key]["buttons"][idx]["shortcut"] = seq_store[0]
            # If value looks like a key sequence, mark type as key
            v = vv.get().strip()
            if v.startswith("<") and v.endswith(">"):
                self.cfg[row_key]["buttons"][idx]["type"] = "key"
        save_config(self.cfg)
        self.on_save()
        self.destroy()


# ── Main app ──────────────────────────────────────────────────────────────────

class ToolbarApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Dual Terminal Shortcut Trigger")
        self.resizable(True, False)
        self.attributes("-topmost", True)
        self.configure(bg="#111111")
        self.overrideredirect(False)

        self.cfg = load_config()
        self._capturing_active = False

        # Restore position
        x = self.cfg.get("window_x", 100)
        y = self.cfg.get("window_y", 100)
        self.geometry(f"+{x}+{y}")

        self._build_menu()
        self._build_ui()
        self._bind_shortcuts()
        self.bind("<Configure>", self._on_move)
        self.protocol("WM_DELETE_WINDOW", self._on_close)
        # Prevent Windows menu bar from stealing Alt key presses
        self.bind("<Alt-KeyPress>", lambda e: "break")

        if not HAS_WIN32:
            self._warn("pywin32 not installed — run: pip install pywin32")
        if not HAS_PYAUTOGUI:
            self._warn("pyautogui not installed — run: pip install pyautogui")

    def _build_menu(self):
        menubar = tk.Menu(self, bg="#1e1e1e", fg="#e0e0e0",
                          activebackground="#E87B2C", activeforeground="white",
                          relief="flat")
        file_menu = tk.Menu(menubar, tearoff=0, bg="#1e1e1e", fg="#e0e0e0",
                            activebackground="#E87B2C", activeforeground="white")
        file_menu.add_command(label="Shortcut Keys",
                              command=self._open_shortcut_prefs)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self._on_close)
        menubar.add_cascade(label="Preferences", menu=file_menu)
        self.config(menu=menubar)

    def _open_shortcut_prefs(self):
        ShortcutPrefsDialog(self, self.cfg, self._rebind_shortcuts)

    def _bind_shortcuts(self):
        for row_key in ("row1", "row2"):
            row_obj = self.row1 if row_key == "row1" else self.row2
            for bdef in self.cfg[row_key]["buttons"]:
                seq = bdef.get("shortcut", "")
                if seq:
                    try:
                        self.bind_all(seq,
                            lambda e, r=row_obj, d=bdef: (
                                None if getattr(self, '_capturing_active', False)
                                else r._fire(d)
                            ))
                    except Exception:
                        pass

    def _rebind_shortcuts(self):
        # Unbind all known shortcuts then rebind
        for row_key in ("row1", "row2"):
            for bdef in self.cfg[row_key]["buttons"]:
                seq = bdef.get("shortcut", "")
                if seq:
                    try:
                        self.unbind_all(seq)
                    except Exception:
                        pass
        self._bind_shortcuts()
        # Refresh button labels on both rows
        for row_obj, row_key in [(self.row1, "row1"), (self.row2, "row2")]:
            for i, bdef in enumerate(self.cfg[row_key]["buttons"]):
                if i < len(row_obj.btns):
                    row_obj.btns[i].config(text=bdef["label"])

    def _build_ui(self):
        BG = "#111111"

        # Thin title bar with drag
        title_bar = tk.Frame(self, bg="#1a1a1a", height=22)
        title_bar.pack(fill="x")
        tk.Label(title_bar, text="⚡ Dual Terminal Shortcut Trigger",
                 bg="#1a1a1a", fg="#888888",
                 font=("Segoe UI", 8)).pack(side="left", padx=8)
        tk.Button(title_bar, text="✕", bg="#1a1a1a", fg="#888888",
                  relief="flat", font=("Segoe UI", 8), cursor="hand2",
                  activebackground="#cc3333", activeforeground="white",
                  command=self._on_close).pack(side="right", padx=4)
        title_bar.bind("<B1-Motion>", self._drag)
        title_bar.bind("<ButtonPress-1>", self._drag_start)

        self._drag_x = 0
        self._drag_y = 0

        # Two rows
        self.row1 = ToolbarRow(self, "row1", self.cfg, self,
                               color="#3B9FD4", bg="#1a1a1a")
        self.row1.pack(fill="x", pady=(2, 1))

        tk.Frame(self, bg="#2a2a2a", height=1).pack(fill="x")

        self.row2 = ToolbarRow(self, "row2", self.cfg, self,
                               color="#CC4444", bg="#1a1a1a")
        self.row2.pack(fill="x", pady=(1, 4))

    def _warn(self, msg):
        tk.Label(self, text=f"⚠ {msg}", bg="#111111", fg="#ff9900",
                 font=("Segoe UI", 8)).pack(fill="x", padx=8, pady=2)

    def _drag_start(self, event):
        self._drag_x = event.x
        self._drag_y = event.y

    def _drag(self, event):
        x = self.winfo_x() + event.x - self._drag_x
        y = self.winfo_y() + event.y - self._drag_y
        self.geometry(f"+{x}+{y}")

    def _on_move(self, event):
        self.cfg["window_x"] = self.winfo_x()
        self.cfg["window_y"] = self.winfo_y()

    def _on_close(self):
        save_config(self.cfg)
        self.destroy()


if __name__ == "__main__":
    app = ToolbarApp()
    app.mainloop()
