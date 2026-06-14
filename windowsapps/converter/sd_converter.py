"""
DeskMediaDevice — SD Card Content Converter
Converts music and video files to the correct format for the SD card.

Requirements:
  - ffmpeg.exe must be in the same folder as this script (or the .exe)

Music output  → /sdcard/music/  : 32kHz stereo 128kbps MP3 + 0.5s adelay, metadata stripped, numbered "01 - Title.mp3"
Video output  → /sdcard/video/  : 480x800 MJPEG @ 20fps + 32kHz stereo 128kbps MP3, metadata stripped (no adelay)
                                   Note: bumper videos must be under 5MB.
"""

import sys
import re
import os
import subprocess
import time
import tempfile
import threading
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

# ── Locate ffmpeg next to this script / frozen exe ───────────────────────────
if getattr(sys, 'frozen', False):
    BASE_DIR = os.path.dirname(sys.executable)
else:
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))

FFMPEG = os.path.join(BASE_DIR, "ffmpeg.exe")


def ffmpeg_available():
    return os.path.isfile(FFMPEG)


# ── Conversion helpers ────────────────────────────────────────────────────────

def run_ffmpeg(args, log_cb):
    """Run ffmpeg with given args list, stream output to log_cb."""
    # -nostdin: never let ffmpeg block waiting for a "Press [q] to stop" keypress.
    cmd = [FFMPEG, "-nostdin", "-y"] + args
    log_cb(f"Running: {' '.join(cmd)}\n")
    # NO PIPE: ffmpeg writes its output to a temp file; we poll that file and
    # stream new content to the log. Pipes under pythonw on Windows can deadlock
    # on ffmpeg's slow, \r-terminated progress output (especially during the
    # loudnorm analysis pass), which made the app appear frozen. A temp log file
    # removes pipes entirely, so there is nothing to deadlock on.
    log_fd, log_path = tempfile.mkstemp(prefix="ffmpeg_", suffix=".log")

    def stream_log(stop_evt):
        """Tail the temp log file and forward new lines to the GUI."""
        pos = 0
        carry = ""
        while True:
            try:
                with open(log_path, "r", encoding="utf-8", errors="replace") as fh:
                    fh.seek(pos)
                    data = fh.read()
                    pos = fh.tell()
            except OSError:
                data = ""
            if data:
                # ffmpeg uses \r for progress; treat it like a newline
                carry += data.replace("\r", "\n")
                *lines, carry = carry.split("\n")
                for ln in lines:
                    log_cb(ln + "\n")
            if stop_evt.is_set():
                # final drain
                try:
                    with open(log_path, "r", encoding="utf-8", errors="replace") as fh:
                        fh.seek(pos)
                        rest = fh.read()
                except OSError:
                    rest = ""
                rest = (carry + rest).replace("\r", "\n")
                for ln in rest.split("\n"):
                    if ln:
                        log_cb(ln + "\n")
                return
            time.sleep(0.1)

    stop_evt = threading.Event()
    with os.fdopen(log_fd, "w", encoding="utf-8", errors="replace") as out_fh:
        proc = subprocess.Popen(
            cmd,
            stdin=subprocess.DEVNULL,   # detach stdin so ffmpeg can't hang on it
            stdout=out_fh,
            stderr=subprocess.STDOUT,
            creationflags=subprocess.CREATE_NO_WINDOW if sys.platform == "win32" else 0
        )
        reader = threading.Thread(target=stream_log, args=(stop_evt,), daemon=True)
        reader.start()
        proc.wait()
        stop_evt.set()
        reader.join(timeout=2)

    try:
        os.remove(log_path)
    except OSError:
        pass
    return proc.returncode


def convert_music(input_path, output_dir, track_num, title, log_cb):
    """Convert a music file to 32kHz stereo 128kbps MP3 with 0.5s adelay,
    loudness-normalized to -16 LUFS / -3.5 dBTP."""
    # If the title already starts with a "NN - " track prefix, don't add another.
    if re.match(r"^\d{1,2}\s*-\s*", title):
        out_name = f"{title}.mp3"
    else:
        out_name = f"{track_num:02d} - {title}.mp3"
    out_path = os.path.join(output_dir, "music", out_name)
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    ret = run_ffmpeg([
        "-i", input_path,
        "-map", "0:a",
        "-map_metadata", "-1",
        "-af", "loudnorm=I=-16:TP=-3.5:LRA=11,adelay=500|500",
        "-ac", "2",
        "-ar", "32000",
        "-ab", "128k",
        "-codec:a", "libmp3lame",
        out_path
    ], log_cb)
    if ret == 0:
        log_cb(f"✓ Music done: {out_name}\n\n")
    else:
        log_cb(f"✗ FAILED: {input_path}\n\n")
    return ret == 0


def convert_video(input_path, output_dir, track_num, title, log_cb):
    """Convert a video to MJPEG + MP3 pair (no adelay). Filename is the title only."""
    base = title
    video_out = os.path.join(output_dir, "video", base + ".mjpeg")
    audio_out = os.path.join(output_dir, "video", base + ".mp3")
    os.makedirs(os.path.dirname(video_out), exist_ok=True)

    # Video pass
    ret = run_ffmpeg([
        "-i", input_path,
        "-c:v", "mjpeg",
        "-q:v", "9",
        "-vf", "transpose=1,scale=480:800",
        "-r", "20",
        "-an",
        video_out
    ], log_cb)
    if ret != 0:
        log_cb(f"✗ Video pass FAILED: {input_path}\n\n")
        return False

    # Audio pass
    ret = run_ffmpeg([
        "-i", input_path,
        "-vn",
        "-map", "0:a",
        "-map_metadata", "-1",
        "-af", "loudnorm=I=-16:TP=-3.5:LRA=11",
        "-ac", "2",
        "-ar", "32000",
        "-ab", "128k",
        "-codec:a", "libmp3lame",
        audio_out
    ], log_cb)
    if ret == 0:
        log_cb(f"✓ Video done: {base}\n\n")
    else:
        log_cb(f"✗ Audio pass FAILED: {input_path}\n\n")
    return ret == 0


# ── GUI ───────────────────────────────────────────────────────────────────────

class ConverterApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("DeskMediaDevice — SD Card Converter")
        self.resizable(False, False)
        self.configure(bg="#1e1e1e")

        self.output_dir = tk.StringVar(value="")
        self.mode = tk.StringVar(value="music")  # music | video

        # Queue of (input_path, track_num, title) tuples
        self.queue = []
        self.running = False

        self._build_ui()
        self._check_ffmpeg()

    # ── UI construction ───────────────────────────────────────────────────────

    def _build_ui(self):
        PAD = 10
        BG = "#1e1e1e"
        FG = "#e0e0e0"
        ACC = "#E87B2C"
        ENTRY_BG = "#2d2d2d"
        FONT = ("Segoe UI", 10)
        FONT_B = ("Segoe UI", 10, "bold")

        def label(parent, text, **kw):
            kw.setdefault("fg", FG)
            kw.setdefault("font", FONT)
            return tk.Label(parent, text=text, bg=BG, **kw)

        def entry(parent, textvariable, width=40):
            return tk.Entry(parent, textvariable=textvariable, width=width,
                            bg=ENTRY_BG, fg=FG, insertbackground=FG,
                            relief="flat", font=FONT)

        def btn(parent, text, cmd, width=12):
            return tk.Button(parent, text=text, command=cmd, width=width,
                             bg=ACC, fg="white", relief="flat", font=FONT_B,
                             activebackground="#cc6520", activeforeground="white",
                             cursor="hand2")

        # ── Mode selector ─────────────────────────────────────────────────────
        mode_frame = tk.Frame(self, bg=BG, pady=PAD, padx=PAD)
        mode_frame.pack(fill="x")
        label(mode_frame, "Mode:").pack(side="left", padx=(0, 8))
        for val, txt in [("music", "Music"), ("video", "Video")]:
            rb = tk.Radiobutton(mode_frame, text=txt, variable=self.mode, value=val,
                                bg=BG, fg=FG, selectcolor="#3a3a3a", activebackground=BG,
                                activeforeground=FG, font=FONT, command=self._on_mode_change)
            rb.pack(side="left", padx=4)

        # ── Mode description ──────────────────────────────────────────────────
        self.mode_desc = label(self, "", fg="#aaaaaa", anchor="w", justify="left",
                               wraplength=560)
        self.mode_desc.pack(fill="x", padx=PAD, pady=(0, 4))

        # ── Output folder ─────────────────────────────────────────────────────
        out_frame = tk.Frame(self, bg=BG, padx=PAD)
        out_frame.pack(fill="x", pady=4)
        label(out_frame, "Output folder:").pack(side="left", padx=(0, 6))
        entry(out_frame, self.output_dir, width=38).pack(side="left")
        btn(out_frame, "Browse…", self._browse_output, width=8).pack(side="left", padx=6)

        # ── File queue ────────────────────────────────────────────────────────
        queue_frame = tk.Frame(self, bg=BG, padx=PAD)
        queue_frame.pack(fill="x", pady=6)
        label(queue_frame, "Files to convert:").pack(anchor="w")

        cols_frame = tk.Frame(queue_frame, bg=BG)
        cols_frame.pack(fill="x", pady=4)

        # Track # (Music only)
        self.track_label = label(cols_frame, "Track #")
        self.track_label.pack(side="left", padx=(0, 6))
        self.track_num_var = tk.StringVar(value="1")
        self.track_entry = tk.Spinbox(cols_frame, from_=1, to=99, width=4,
                                      textvariable=self.track_num_var,
                                      bg=ENTRY_BG, fg=FG, buttonbackground="#3a3a3a",
                                      relief="flat", font=FONT)
        self.track_entry.pack(side="left", padx=(0, 12))

        self.title_label = label(cols_frame, "Title")
        self.title_label.pack(side="left", padx=(0, 6))
        self.title_var = tk.StringVar()
        self.title_entry = entry(cols_frame, self.title_var, width=24)
        self.title_entry.pack(side="left", padx=(0, 10))

        btn(cols_frame, "Add file…", self._add_file, width=8).pack(side="left", padx=4)
        btn(cols_frame, "Clear", self._clear_queue, width=6).pack(side="left", padx=2)

        # Listbox
        list_frame = tk.Frame(queue_frame, bg=BG)
        list_frame.pack(fill="x", pady=4)
        self.listbox = tk.Listbox(list_frame, bg=ENTRY_BG, fg=FG, font=FONT,
                                  selectbackground=ACC, relief="flat",
                                  width=70, height=6)
        self.listbox.pack(side="left", fill="x", expand=True)
        sb = tk.Scrollbar(list_frame, orient="vertical", command=self.listbox.yview)
        sb.pack(side="right", fill="y")
        self.listbox.config(yscrollcommand=sb.set)

        # ── Convert button ────────────────────────────────────────────────────
        self.convert_btn = btn(self, "Convert All", self._start_conversion, width=14)
        self.convert_btn.pack(pady=8)

        # ── Log output ────────────────────────────────────────────────────────
        log_frame = tk.Frame(self, bg=BG, padx=PAD, pady=4)
        log_frame.pack(fill="both", expand=True)
        label(log_frame, "Log:").pack(anchor="w")
        self.log = tk.Text(log_frame, bg=ENTRY_BG, fg="#cccccc", font=("Consolas", 9),
                           relief="flat", height=12, state="disabled",
                           wrap="word")
        self.log.pack(fill="both", expand=True)
        log_sb = tk.Scrollbar(log_frame, orient="vertical", command=self.log.yview)
        log_sb.pack(side="right", fill="y")
        self.log.config(yscrollcommand=log_sb.set)

        self._on_mode_change()

    # ── Mode change ───────────────────────────────────────────────────────────

    def _on_mode_change(self):
        mode = self.mode.get()
        descs = {
            "music":  "Music: converts to 32kHz stereo 128kbps MP3 with 0.5s silence, metadata stripped. Output → output_folder/music/",
            "video":  "Video: converts to 480×800 MJPEG @ 20fps + 32kHz MP3, metadata stripped. Output → output_folder/video/   ⚠ Bumper videos must be under 5MB.",
        }
        self.mode_desc.config(text=descs[mode])

        # Track # only applies to Music — hide the field entirely otherwise
        is_music = (mode == "music")
        if is_music:
            self.track_label.pack(side="left", padx=(0, 6), before=self.title_label)
            self.track_entry.pack(side="left", padx=(0, 12), before=self.title_label)
        else:
            self.track_label.pack_forget()
            self.track_entry.pack_forget()

        self._clear_queue()

    # ── File management ───────────────────────────────────────────────────────

    def _add_file(self):
        mode = self.mode.get()
        if mode == "music":
            filetypes = [("Audio files", "*.mp3 *.wav *.flac *.aac *.m4a"), ("All files", "*.*")]
        else:
            filetypes = [("Video files", "*.mp4 *.mov *.avi *.mkv *.m4v"), ("All files", "*.*")]

        paths = filedialog.askopenfilenames(filetypes=filetypes)
        if not paths:
            return

        for path in paths:
            title = self.title_var.get().strip()
            if not title:
                # Default title from filename
                title = os.path.splitext(os.path.basename(path))[0]

            if mode == "music":
                try:
                    track = int(self.track_num_var.get())
                except ValueError:
                    track = len(self.queue) + 1
                # Auto-increment track number
                self.track_num_var.set(str(track + 1))
            else:  # video — number sequentially by queue position
                track = len(self.queue) + 1

            entry = {"path": path, "mode": mode, "track": track, "title": title}
            if mode == "music":
                display = f"[music] {track:02d} - {title}  ← {os.path.basename(path)}"
            else:
                display = f"[video] {title}  ← {os.path.basename(path)}"

            self.queue.append(entry)
            self.listbox.insert("end", display)

        self.title_var.set("")

    def _clear_queue(self):
        self.queue.clear()
        self.listbox.delete(0, "end")

    # ── Output folder ─────────────────────────────────────────────────────────

    def _browse_output(self):
        d = filedialog.askdirectory(title="Select output folder")
        if d:
            self.output_dir.set(d)

    # ── Conversion ────────────────────────────────────────────────────────────

    def _log(self, text):
        self.log.config(state="normal")
        self.log.insert("end", text)
        self.log.see("end")
        self.log.config(state="disabled")
        self.update_idletasks()

    def _start_conversion(self):
        if self.running:
            return
        if not ffmpeg_available():
            messagebox.showerror("FFmpeg not found",
                                 f"ffmpeg.exe not found at:\n{FFMPEG}\n\nPlace ffmpeg.exe in the same folder as this app.")
            return
        if not self.output_dir.get():
            messagebox.showerror("No output folder", "Please select an output folder first.")
            return
        if not self.queue:
            messagebox.showinfo("Nothing to convert", "Add files to the queue first.")
            return

        self.running = True
        self.convert_btn.config(state="disabled", text="Converting…")
        thread = threading.Thread(target=self._run_conversions, daemon=True)
        thread.start()

    def _run_conversions(self):
        output_dir = self.output_dir.get()
        success = 0
        fail = 0

        for item in self.queue:
            mode = item["mode"]
            path = item["path"]
            track = item["track"]
            title = item["title"]

            self._log(f"\u2500\u2500 {os.path.basename(path)} \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\n")

            if mode == "music":
                ok = convert_music(path, output_dir, track, title, self._log)
            elif mode == "video":
                ok = convert_video(path, output_dir, track, title, self._log)
            else:
                ok = False

            if ok:
                success += 1
            else:
                fail += 1

        self._log(f"\n{'='*50}\n")
        self._log(f"Done. {success} succeeded, {fail} failed.\n")
        self._log(f"Output folder: {output_dir}\n")

        self.running = False
        self.convert_btn.config(state="normal", text="Convert All")

    # \u2500\u2500 FFmpeg check \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500

    def _check_ffmpeg(self):
        if not ffmpeg_available():
            self._log(f"WARNING: ffmpeg.exe not found at {FFMPEG}\n")
            self._log("Download ffmpeg.exe and place it in the same folder as this app.\n\n")
        else:
            self._log(f"FFmpeg found: {FFMPEG}\n\n")


if __name__ == "__main__":
    app = ConverterApp()
    app.mainloop()
