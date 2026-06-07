"""
DeskMediaDevice — Dual Terminal Launcher
Left:  standard PowerShell
Right: specialized shell (ESP-IDF or any configured launcher)

Alt+1  = Left CLS,  Alt+2–6  = Left programmable buttons 1–5
Alt+7  = Right CLS, Alt+8–12 = Right programmable buttons 1–5
  (Alt+10 = Alt+0, Alt+11 = Alt+-, Alt+12 = Alt+=)

Config saved to dual_terminal_config.json next to this script.
"""

import sys
import os
import json
import subprocess
import threading
import tkinter as tk
from tkinter import filedialog, messagebox

# ── Config ────────────────────────────────────────────────────────────────────

if getattr(sys, 'frozen', False):
    BASE_DIR = os.path.dirname(sys.executable)
else:
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))

CONFIG_FILE = os.path.join(BASE_DIR, "dual_terminal_config.json")

ESP_IDF_LAUNCH = (
    'cmd /k "C:\\Espressif\\idf_cmd_init.bat && '
    'cd /d C:\\Users\\Administrator\\Documents\\DeskMediaDeviceV2"'
)

DEFAULT_CONFIG = {
    "left_folder":      "C:\\",
    "right_folder":     "C:\\Users\\Administrator\\Documents\\DeskMediaDeviceV2",
    "right_launch_cmd": ESP_IDF_LAUNCH,
    "left_buttons": [
        {"label": "Git Log",    "cmd": "git log --oneline -10", "alt": 2},
        {"label": "Git Status", "cmd": "git status",            "alt": 3},
        {"label": "Git Add",    "cmd": "git add -A",            "alt": 4},
        {"label": "Dir",        "cmd": "dir",                   "alt": 5},
        {"label": "Custom 5",   "cmd": "",                      "alt": 6},
    ],
    "right_buttons": [
        {"label": "Preflight", "cmd": ".\\preflight_check.bat",                                                                                                              "alt": 8},
        {"label": "Clean",     "cmd": "idf.py fullclean 2>&1 | Out-Null",                                                                                                    "alt": 9},
        {"label": "Build",     "cmd": "idf.py build 2>&1 | ForEach-Object { Write-Host $_; $_ } | Out-File -FilePath \"claudetransferv2\\build_log.txt\" -Encoding UTF8",    "alt": 10},
        {"label": "Flash",     "cmd": "idf.py -p COM4 flash",                                                                                                                "alt": 11},
        {"label": "Parse",     "cmd": ".\\parse_logs.ps1 claudetransferv2\\build_log.txt build",                                                                             "alt": 12},
    ],
}


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
    return dict(DEFAULT_CONFIG)


def save_config(cfg):
    with open(CONFIG_FILE, "w") as f:
        json.dump(cfg, f, indent=2)


# ── Terminal panel ────────────────────────────────────────────────────────────

PROMPT_MARKER = "##PSPROMPT##"   # hidden marker PowerShell writes before each prompt


class TerminalPanel(tk.Frame):

    def __init__(self, parent, side, cfg, app, **kwargs):
        super().__init__(parent, **kwargs)
        self.side   = side
        self.cfg    = cfg
        self.app    = app
        self.process = None
        self.running = False
        self._input_buf = ""        # current typed input
        self._input_start = "1.0"  # text index where input begins
        self._history = []
        self._hist_idx = -1

        BG       = "#1a1a1a"
        FG       = "#e0e0e0"
        ACC      = "#7F77DD"
        BTN_BG   = "#2a2a2a"
        ENTRY_BG = "#252525"
        FONT     = ("Segoe UI", 9)
        FONT_B   = ("Segoe UI", 9, "bold")
        MONO     = ("Consolas", 10)

        self.configure(bg=BG)

        # ── Title ─────────────────────────────────────────────────────────────
        title = "PowerShell" if side == "left" else "ESP-IDF / Special"
        tk.Label(self, text=title, bg="#111111", fg=ACC,
                 font=("Segoe UI", 10, "bold"), anchor="w",
                 padx=8, pady=4).pack(fill="x")

        # ── Launch command (right only) ───────────────────────────────────────
        if side == "right":
            lc = tk.Frame(self, bg=BG, pady=3)
            lc.pack(fill="x", padx=6)
            tk.Label(lc, text="Launch:", bg=BG, fg="#888888", font=FONT).pack(side="left", padx=(0,4))
            self.launch_var = tk.StringVar(value=cfg.get("right_launch_cmd", ""))
            tk.Entry(lc, textvariable=self.launch_var, bg=ENTRY_BG, fg=FG,
                     insertbackground=FG, relief="flat",
                     font=("Consolas", 8)).pack(side="left", fill="x", expand=True, padx=(0,4))
            tk.Button(lc, text="Launch", bg=ACC, fg="white", relief="flat",
                      font=FONT_B, cursor="hand2", activebackground="#6a62c4",
                      command=self._launch_special).pack(side="left")

        # ── Folder row ────────────────────────────────────────────────────────
        fold = tk.Frame(self, bg=BG, pady=2)
        fold.pack(fill="x", padx=6)
        tk.Label(fold, text="Folder:", bg=BG, fg="#888888", font=FONT).pack(side="left", padx=(0,4))
        self.folder_var = tk.StringVar(value=cfg.get(f"{side}_folder", "C:\\"))
        tk.Entry(fold, textvariable=self.folder_var, bg=ENTRY_BG, fg=FG,
                 insertbackground=FG, relief="flat",
                 font=("Consolas", 8)).pack(side="left", fill="x", expand=True, padx=(0,4))
        tk.Button(fold, text="…", bg=BTN_BG, fg=FG, relief="flat",
                  font=FONT, width=2, cursor="hand2",
                  command=self._browse_folder).pack(side="left", padx=(0,4))
        tk.Button(fold, text="CD", bg=BTN_BG, fg=ACC, relief="flat",
                  font=FONT_B, width=3, cursor="hand2",
                  command=self._send_cd).pack(side="left")

        # ── Button bar ────────────────────────────────────────────────────────
        btn_frame = tk.Frame(self, bg=BG, pady=3)
        btn_frame.pack(fill="x", padx=6)
        self._btn_frame = btn_frame

        cls_alt = 1 if side == "left" else 7
        tk.Button(btn_frame, text=f"CLS [Alt+{cls_alt}]",
                  bg=BTN_BG, fg="#ff6666", relief="flat", font=FONT, cursor="hand2",
                  command=self._clear).pack(side="left", padx=(0,4))

        self.prog_btns = []
        self._build_prog_buttons()

        # ── Terminal Text widget ──────────────────────────────────────────────
        out_frame = tk.Frame(self, bg=BG)
        out_frame.pack(fill="both", expand=True, padx=6, pady=(2,6))

        self.term = tk.Text(out_frame, bg="#0d0d0d", fg="#cccccc",
                            font=MONO, relief="flat", wrap="char",
                            insertbackground="#ffffff", insertwidth=2,
                            cursor="xterm")
        self.term.pack(side="left", fill="both", expand=True)

        sb = tk.Scrollbar(out_frame, orient="vertical", command=self.term.yview, bg=BG)
        sb.pack(side="right", fill="y")
        self.term.config(yscrollcommand=sb.set)

        # Tags
        self.term.tag_config("prompt",  foreground="#7F77DD")
        self.term.tag_config("banner",  foreground="#555555")
        self.term.tag_config("input",   foreground="#ffffff")
        self.term.tag_config("output",  foreground="#cccccc")
        self.term.tag_config("error",   foreground="#ff6666")

        # Key bindings — intercept all typing
        self.term.bind("<Key>",          self._on_key)
        self.term.bind("<Return>",       self._on_enter)
        self.term.bind("<BackSpace>",    self._on_backspace)
        self.term.bind("<Delete>",       self._on_backspace)
        self.term.bind("<Up>",           self._on_hist_up)
        self.term.bind("<Down>",         self._on_hist_down)
        self.term.bind("<Home>",         self._on_home)
        self.term.bind("<Control-c>",    self._on_ctrl_c)
        # Prevent mouse from moving cursor behind the prompt
        self.term.bind("<Button-1>",     self._on_click)
        # Block paste behind prompt
        self.term.bind("<Control-v>",    self._on_paste)
        self.term.bind("<<Paste>>",      self._on_paste)

        # Start shell
        if side == "left":
            self._start_powershell()

    # ── Process ───────────────────────────────────────────────────────────────

    def _start_powershell(self):
        self._kill_process()
        folder = self.folder_var.get()
        cwd = folder if os.path.isdir(folder) else None
        # Use -NonInteractive so PS doesn't try to write its own prompt
        # We handle prompt display ourselves
        try:
            escaped = folder.replace("'", "''")
            self.process = subprocess.Popen(
                ["powershell.exe", "-NoLogo", "-NoExit", "-Command",
                 f"Set-Location '{escaped}'"],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                cwd=cwd,
                creationflags=subprocess.CREATE_NO_WINDOW,
                bufsize=1,
            )
            self.running = True
            threading.Thread(target=self._read_output, daemon=True).start()
            self.after(500, self._write_prompt)
        except Exception as ex:
            self._append(f"[Error: {ex}]\n", "error")

    def _launch_special(self):
        cmd = self.launch_var.get().strip()
        if not cmd:
            messagebox.showinfo("No command", "Set a launch command first.")
            return
        folder = self.folder_var.get()
        cwd = folder if os.path.isdir(folder) else None
        subprocess.Popen(cmd, shell=True, cwd=cwd)
        self._append(f"[Launched external: {cmd}]\n", "banner")

    def _kill_process(self):
        self.running = False
        if self.process:
            try:
                self.process.terminate()
            except Exception:
                pass
            self.process = None

    # ── Output reader ─────────────────────────────────────────────────────────

    def _read_output(self):
        try:
            for line in self.process.stdout:
                if not self.running:
                    break
                self._append(line, "output")
        except Exception:
            pass
        if self.running:
            self._append("\n[Process ended unexpectedly]\n", "error")

    # ── Terminal I/O ──────────────────────────────────────────────────────────

    def _append(self, text, tag="output"):
        """Append text to terminal."""
        def _do():
            self.term.config(state="normal")
            self.term.insert("end", text, tag)
            self.term.see("end")
            self.term.config(state="disabled")
        self.after(0, _do)

    def _write_prompt(self):
        """Write a prompt line and position cursor for input."""
        def _do():
            self.term.config(state="normal")
            # Ensure prompt starts on a new line
            last = self.term.get("end-2c", "end-1c")
            if last and last != "\n":
                self.term.insert("end", "\n")
            pt = f"PS {self.folder_var.get()}> "
            self.term.insert("end", pt, "prompt")
            self._set_input_start()
            self.term.see("end")
        self.after(0, _do)

    def _set_input_start(self):
        self._input_buf = ""
        self._input_start = self.term.index("end-1c")
        self.term.mark_set("input_start", self._input_start)
        self.term.mark_gravity("input_start", "left")
        self.term.config(state="normal")

    def _get_input_text(self):
        try:
            return self.term.get("input_start", "end-1c")
        except Exception:
            return ""

    def _replace_input(self, text):
        """Replace current input line with text."""
        try:
            self.term.delete("input_start", "end")
        except Exception:
            pass
        self.term.insert("end", text, "input")
        self._input_buf = text

    # ── Key handlers ─────────────────────────────────────────────────────────

    def _on_key(self, event):
        """Route printable characters to input."""
        # Ignore modifier-only keys and special keys handled elsewhere
        if event.keysym in ("Return", "BackSpace", "Delete", "Up", "Down",
                             "Left", "Right", "Home", "End", "Tab",
                             "Escape", "Control_L", "Control_R",
                             "Alt_L", "Alt_R", "Shift_L", "Shift_R"):
            return
        if event.state & 0x4:  # Ctrl held
            return
        if event.char and event.char.isprintable():
            self.term.config(state="normal")
            # Make sure cursor is at end
            self.term.mark_set("insert", "end")
            self.term.insert("end", event.char, "input")
            self._input_buf += event.char
            self.term.see("end")
        return "break"

    def _on_enter(self, event):
        cmd = self._get_input_text()
        self.term.config(state="normal")
        self.term.insert("end", "\n")
        self.term.config(state="disabled")
        if cmd.strip():
            self._history.append(cmd)
            self._hist_idx = len(self._history)
            self._send(cmd)
        else:
            # Empty enter — just show a new prompt
            self._write_prompt()
        self._input_buf = ""
        return "break"

    def _on_backspace(self, event):
        current = self._get_input_text()
        if current:
            self.term.config(state="normal")
            self.term.delete("end-2c", "end-1c")
            self._input_buf = current[:-1]
        return "break"

    def _on_hist_up(self, event):
        if not self._history:
            return "break"
        self._hist_idx = max(0, self._hist_idx - 1)
        self._replace_input(self._history[self._hist_idx])
        return "break"

    def _on_hist_down(self, event):
        if not self._history:
            return "break"
        self._hist_idx = min(len(self._history), self._hist_idx + 1)
        text = self._history[self._hist_idx] if self._hist_idx < len(self._history) else ""
        self._replace_input(text)
        return "break"

    def _on_home(self, event):
        self.term.mark_set("insert", "input_start")
        return "break"

    def _on_ctrl_c(self, event):
        if self.process and self.process.poll() is None:
            try:
                self.process.send_signal(__import__("signal").CTRL_C_EVENT)
            except Exception:
                pass
        return "break"

    def _on_click(self, event):
        # Always keep cursor at end (in input zone)
        self.after(1, lambda: self.term.mark_set("insert", "end"))

    def _on_paste(self, event):
        try:
            text = self.clipboard_get()
            for ch in text:
                if ch == "\n":
                    self._on_enter(None)
                elif ch.isprintable():
                    self.term.config(state="normal")
                    self.term.insert("end", ch, "input")
                    self._input_buf += ch
        except Exception:
            pass
        return "break"

    # ── Send command ──────────────────────────────────────────────────────────

    def _send_raw(self, text):
        """Send to process without showing prompt after."""
        if self.process and self.process.poll() is None:
            try:
                self.process.stdin.write(text + "\n")
                self.process.stdin.flush()
            except Exception:
                pass

    def _send(self, text):
        if self.process and self.process.poll() is None:
            try:
                self.process.stdin.write(text + "\n")
                self.process.stdin.flush()
                # Show prompt after a short delay to let output arrive first
                self.after(200, self._write_prompt)
            except Exception as ex:
                self._append(f"[Send error: {ex}]\n", "error")
        else:
            self._append("[No process — restarting...]\n", "banner")
            self._start_powershell()

    def _send_cmd(self, cmd):
        """Called from buttons / Alt keys."""
        if not cmd:
            return
        self._replace_input(cmd)
        self._on_enter(None)

    def _send_cls(self):
        self._clear()

    def _clear(self):
        self.term.config(state="normal")
        self.term.delete("1.0", "end")
        self._write_prompt()

    def _send_cd(self):
        folder = self.folder_var.get()
        self._send_cmd(f"Set-Location '{folder}'")
        self._save_folder()
        # Update prompt to reflect new folder
        self.after(300, self._write_prompt)

    def _browse_folder(self):
        d = filedialog.askdirectory(title="Select folder")
        if d:
            self.folder_var.set(d.replace("/", "\\"))
            self._save_folder()

    def _save_folder(self):
        self.cfg[f"{self.side}_folder"] = self.folder_var.get()
        save_config(self.cfg)

    # ── Button helpers ────────────────────────────────────────────────────────

    def _btn_label(self, bdef):
        alt = bdef.get("alt", "?")
        label = bdef.get("label", "")
        return f"{label}\n[Alt+{alt}]"

    def _build_prog_buttons(self):
        BG   = "#1a1a1a"
        FG   = "#e0e0e0"
        FONT = ("Segoe UI", 9)
        btn_key = f"{self.side}_buttons"
        for bdef in self.cfg.get(btn_key, []):
            b = tk.Button(self._btn_frame, text=self._btn_label(bdef),
                          bg="#2a2a2a", fg=FG, relief="flat",
                          font=FONT, cursor="hand2",
                          command=lambda d=bdef: self._send_cmd(d["cmd"]))
            b.pack(side="left", padx=2)
            self.prog_btns.append(b)

    def refresh_buttons(self):
        for b in self.prog_btns:
            b.destroy()
        self.prog_btns.clear()
        self._build_prog_buttons()
        if self.side == "right" and hasattr(self, "launch_var"):
            self.launch_var.set(self.cfg.get("right_launch_cmd", ""))


# ── Preferences dialog ────────────────────────────────────────────────────────

class PrefsDialog(tk.Toplevel):
    def __init__(self, parent, cfg, section):
        super().__init__(parent)
        self.cfg     = cfg
        self.section = section
        self.title(f"Preferences — {'Normal' if section == 'left_buttons' else 'Special'} terminal buttons")
        self.resizable(False, False)
        self.configure(bg="#1e1e1e")
        self.grab_set()

        BG       = "#1e1e1e"
        FG       = "#e0e0e0"
        ENTRY_BG = "#2d2d2d"
        ACC      = "#7F77DD"
        FONT     = ("Segoe UI", 9)
        FONT_B   = ("Segoe UI", 9, "bold")

        if section == "right_buttons":
            lc = tk.Frame(self, bg=BG, pady=6, padx=10)
            lc.pack(fill="x")
            tk.Label(lc, text="Launch command:", bg=BG, fg=FG, font=FONT_B).pack(anchor="w")
            self.launch_var = tk.StringVar(value=cfg.get("right_launch_cmd", ""))
            tk.Entry(lc, textvariable=self.launch_var, width=70,
                     bg=ENTRY_BG, fg=FG, insertbackground=FG,
                     relief="flat", font=("Consolas", 8)).pack(fill="x", pady=2)

        hdr = tk.Frame(self, bg=BG, padx=10)
        hdr.pack(fill="x", pady=(6, 0))
        for col, w in [("Label", 12), ("Command", 44), ("Alt #", 6)]:
            tk.Label(hdr, text=col, bg=BG, fg="#888888",
                     font=FONT, width=w, anchor="w").pack(side="left", padx=2)

        self.rows = []
        for bdef in cfg.get(section, []):
            row = tk.Frame(self, bg=BG, padx=10, pady=2)
            row.pack(fill="x")
            lv = tk.StringVar(value=bdef.get("label", ""))
            cv = tk.StringVar(value=bdef.get("cmd", ""))
            av = tk.StringVar(value=str(bdef.get("alt", "")))
            tk.Entry(row, textvariable=lv, width=12, bg=ENTRY_BG, fg=FG,
                     insertbackground=FG, relief="flat", font=FONT).pack(side="left", padx=2)
            tk.Entry(row, textvariable=cv, width=44, bg=ENTRY_BG, fg=FG,
                     insertbackground=FG, relief="flat", font=("Consolas", 8)).pack(side="left", padx=2)
            tk.Entry(row, textvariable=av, width=6, bg=ENTRY_BG, fg=FG,
                     insertbackground=FG, relief="flat", font=FONT).pack(side="left", padx=2)
            self.rows.append((lv, cv, av))

        btn_row = tk.Frame(self, bg=BG, pady=10)
        btn_row.pack()
        tk.Button(btn_row, text="Save", bg=ACC, fg="white", relief="flat",
                  font=FONT_B, width=10, cursor="hand2",
                  command=self._save).pack(side="left", padx=6)
        tk.Button(btn_row, text="Cancel", bg="#333333", fg=FG, relief="flat",
                  font=FONT, width=8, cursor="hand2",
                  command=self.destroy).pack(side="left", padx=6)

    def _save(self):
        buttons = self.cfg.get(self.section, [])
        for i, (lv, cv, av) in enumerate(self.rows):
            if i < len(buttons):
                buttons[i]["label"] = lv.get()
                buttons[i]["cmd"]   = cv.get()
                try:
                    buttons[i]["alt"] = int(av.get())
                except ValueError:
                    pass
        self.cfg[self.section] = buttons
        if self.section == "right_buttons" and hasattr(self, "launch_var"):
            self.cfg["right_launch_cmd"] = self.launch_var.get()
        save_config(self.cfg)
        self.destroy()


# ── Main app ──────────────────────────────────────────────────────────────────

class DualTerminalApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("DeskMediaDevice — Dual Terminal")
        self.geometry("1400x800")
        self.minsize(800, 500)
        self.configure(bg="#111111")

        self.cfg = load_config()
        self._build_menu()
        self._build_panels()
        self._bind_alt_keys()
        self.protocol("WM_DELETE_WINDOW", self._on_close)

    def _build_menu(self):
        menubar = tk.Menu(self, bg="#1e1e1e", fg="#e0e0e0",
                          activebackground="#7F77DD", activeforeground="white",
                          relief="flat")
        file_menu = tk.Menu(menubar, tearoff=0, bg="#1e1e1e", fg="#e0e0e0",
                            activebackground="#7F77DD", activeforeground="white")
        file_menu.add_command(label="Exit", command=self._on_close)
        menubar.add_cascade(label="File", menu=file_menu)

        prefs_menu = tk.Menu(menubar, tearoff=0, bg="#1e1e1e", fg="#e0e0e0",
                             activebackground="#7F77DD", activeforeground="white")
        prefs_menu.add_command(label="Normal terminal buttons",
                               command=lambda: self._open_prefs("left_buttons"))
        prefs_menu.add_command(label="Special terminal buttons",
                               command=lambda: self._open_prefs("right_buttons"))
        menubar.add_cascade(label="Preferences", menu=prefs_menu)
        self.config(menu=menubar)

    def _open_prefs(self, section):
        dlg = PrefsDialog(self, self.cfg, section)
        self.wait_window(dlg)
        if section == "left_buttons":
            self.left_panel.refresh_buttons()
        else:
            self.right_panel.refresh_buttons()
        self.rebind_alt_keys()

    def _build_panels(self):
        c = tk.Frame(self, bg="#111111")
        c.pack(fill="both", expand=True)
        c.grid_columnconfigure(0, weight=1, uniform="half")
        c.grid_columnconfigure(1, weight=1, uniform="half")
        c.grid_rowconfigure(0, weight=1)

        self.left_panel  = TerminalPanel(c, "left",  self.cfg, self, bg="#1a1a1a")
        self.right_panel = TerminalPanel(c, "right", self.cfg, self, bg="#1a1a1a")
        self.left_panel.grid(row=0, column=0, sticky="nsew")

        tk.Frame(c, bg="#333333", width=2).grid(row=0, column=0, sticky="nse")

        self.right_panel.grid(row=0, column=1, sticky="nsew")

    # ── Alt key bindings ──────────────────────────────────────────────────────

    ALT_KEYSYMS = {
        1: "1", 2: "2", 3: "3", 4: "4", 5: "5",
        6: "6", 7: "7", 8: "8", 9: "9",
        10: "0", 11: "minus", 12: "equal",
    }

    def _alt_seq(self, n):
        sym = self.ALT_KEYSYMS.get(n)
        return f"<Alt-{sym}>" if sym else None

    def _bind_alt_keys(self):
        self.bind_all(self._alt_seq(1), lambda e: self.left_panel._clear())
        self.bind_all(self._alt_seq(7), lambda e: self.right_panel._clear())

        for bdef in self.cfg.get("left_buttons", []):
            seq = self._alt_seq(bdef.get("alt"))
            cmd = bdef.get("cmd", "")
            if seq:
                self.bind_all(seq, lambda e, c=cmd: self.left_panel._send_cmd(c))

        for bdef in self.cfg.get("right_buttons", []):
            seq = self._alt_seq(bdef.get("alt"))
            cmd = bdef.get("cmd", "")
            if seq:
                self.bind_all(seq, lambda e, c=cmd: self.right_panel._send_cmd(c))

    def rebind_alt_keys(self):
        for n in range(1, 13):
            seq = self._alt_seq(n)
            if seq:
                try:
                    self.unbind_all(seq)
                except Exception:
                    pass
        self._bind_alt_keys()

    def _on_close(self):
        self.left_panel._kill_process()
        self.right_panel._kill_process()
        save_config(self.cfg)
        self.destroy()


if __name__ == "__main__":
    app = DualTerminalApp()
    app.mainloop()
