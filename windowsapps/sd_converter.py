"""
DeskMediaDevice — SD Card Content Converter
Converts music and video files to the correct format for the SD card.

Requirements:
  - ffmpeg.exe must be in the same folder as this script (or the .exe)

Music output  → /sdcard/music/  : 32kHz stereo 128kbps MP3 + 0.5s adelay, numbered "01 - Title.mp3"
Video output  → /sdcard/video/  : 480x800 MJPEG @ 20fps + 32kHz stereo 128kbps MP3 (no adelay)
Bumper output → /sdcard/video/  : same as video but saved as bumper.mjpeg / bumper.mp3
"""

import sys
import os
import subprocess
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
    cmd = [FFMPEG, "-y"] + args
    log_cb(f"Running: {' '.join(cmd)}\n")
    proc = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        creationflags=subprocess.CREATE_NO_WINDOW if sys.platform == "win32" else 0
    )
    for line in proc.stdout:
        log_cb(line)
    proc.wait()
    return proc.returncode


def convert_music(input_path, output_dir, track_num, title, log_cb):
    """Convert a music file to 32kHz stereo 128kbps MP3 with 0.5s adelay."""
    out_name = f"{track_num:02d} - {title}.mp3"
    out_path = os.path.join(output_dir, "music", out_name)
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    ret = run_ffmpeg([
        "-i", input_path,
        "-af", "adelay=500|500",
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
    """Convert a video to MJPEG + MP3 pair (no adelay)."""
    base = f"{track_num:02d} - {title}"
    video_out = os.path.join(output_dir, "video", base + ".mjpeg")
    audio_out = os.path.join(output_dir, "video", base + ".mp3")
    os.makedirs(os.path.dirname(video_out), exist_ok=True)

    # Video pass
    ret = run_ffmpeg([
        "-i", input_path,
        "-c:v", "mjpeg",
        "-q:v", "5",
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


def convert_bumper(input_path, output_dir, log_cb):
    """Convert a video to bumper.mjpeg + bumper.mp3 (keep under 5MB, no adelay)."""
    video_out = os.path.join(output_dir, "video", "bumper.mjpeg")
    audio_out = os.path.join(output_dir, "video", "bumper.mp3")
    os.makedirs(os.path.dirname(video_out), exist_ok=True)

    ret = run_ffmpeg([
        "-i", input_path,
        "-c:v", "mjpeg",
        "-q:v", "5",
        "-vf", "transpose=1,scale=480:800",
        "-r", "20",
        "-an",
        video_out
    ], log_cb)
    if ret != 0:
        log_cb(f"✗ Bumper video FAILED\n\n")
        return False

    ret = run_ffmpeg([
        "-i", input_path,
        "-vn",
        "-ac", "2",
        "-ar", "32000",
        "-ab", "128k",
        "-codec:a", "libmp3lame",
        audio_out
    ], log_cb)
    if ret == 0:
        log_cb(f"✓ Bumper done: bumper.mjpeg + bumper.mp3\n\n")
    else:
        log_cb(f"✗ Bumper audio FAILED\n\n")
    return ret == 0


# ── GUI ───────────────────────────────────────────────────────────────────────

class ConverterApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("DeskMediaDevice — SD Card Converter")
        self.resizable(False, False)
        self.configure(bg="#1e1e1e")

        self.output_dir = tk.StringVar(value="")
        self.mode = tk.StringVar(value="music")  # music | video | bumper

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
        ACC = "#7F77DD"
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
                             activebackground="#6a62c4", activeforeground="white",
                             cursor="hand2")

        # ── Mode selector ─────────────────────────────────────────────────────
        mode_frame = tk.Frame(self, bg=BG, pady=PAD, padx=PAD)
        mode_frame.pack(fill="x")
        label(mode_frame, "Mode:").pack(side="left", padx=(0, 8))
        for val, txt in [("music", "Music"), ("video", "Video"), ("bumper", "Bumper (intro)")]:
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

        # Track # (hidden for bumper)
        self.track_label = label(cols_frame, "Track #")
        self.track_label.pack(side="left", padx=(0, 6))
        self.track_num_var = tk.StringVar(value="1")
        self.track_entry = tk.Spinbox(cols_frame, from_=1, to=99, width=4,
                                      textvariable=self.track_num_var,
                                      bg=ENTRY_BG, fg=FG, buttonbackground="#3a3a3a",
                                      relief="flat", font=FONT)
        self.track_entry.pack(side="left", padx=(0, 12))

        label(cols_frame, "Title").pack(side="left", padx=(0, 6))
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
            "music":  "Music: converts to 32kHz stereo 128kbps MP3 with 0.5s silence. Output → output_folder/music/",
            "video":  "Video: converts to 480×800 MJPEG @ 20fps + 32kHz MP3. Output → output_folder/video/",
            "bumper": "Bumper: same as video but saved as bumper.mjpeg + bumper.mp3. No track number needed.",
        }
        self.mode_desc.config(text=descs[mode])

        is_bumper = (mode == "bumper")
        state = "disabled" if is_bumper else "normal"
        self.track_label.config(fg="#555555" if is_bumper else "#e0e0e0")
        self.track_entry.config(state=state)

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

            if mode == "bumper":
                entry = {"path": path, "mode": "bumper", "track": 0, "title": "bumper"}
                display = f"[bumper] {os.path.basename(path)}"
            else:
                try:
                    track = int(self.track_num_var.get())
                except ValueError:
                    track = len(self.queue) + 1
                entry = {"path": path, "mode": mode, "track": track, "title": title}
                display = f"[{mode}] {track:02d} - {title}  ← {os.path.basename(path)}"
                # Auto-increment track number
                self.track_num_var.set(str(track + 1))

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

            self._log(f"── {os.path.basename(path)} ──────────────────────\n")

            if mode == "music":
                ok = convert_music(path, output_dir, track, title, self._log)
            elif mode == "video":
                ok = convert_video(path, output_dir, track, title, self._log)
            elif mode == "bumper":
                ok = convert_bumper(path, output_dir, self._log)
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

    # ── FFmpeg check ──────────────────────────────────────────────────────────

    def _check_ffmpeg(self):
        if not ffmpeg_available():
            self._log(f"WARNING: ffmpeg.exe not found at {FFMPEG}\n")
            self._log("Download ffmpeg.exe and place it in the same folder as this app.\n\n")
        else:
            self._log(f"FFmpeg found: {FFMPEG}\n\n")


if __name__ == "__main__":
    app = ConverterApp()
    app.mainloop()
