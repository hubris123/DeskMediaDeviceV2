"""
Key debug tool - shows exact tkinter event values for any key press.
Run this, press keys, and paste the output so we can see what Windows reports.
"""
import tkinter as tk

root = tk.Tk()
root.title("Key Debug")
root.geometry("500x400")
root.configure(bg="#1e1e1e")

log = tk.Text(root, bg="#0d0d0d", fg="#cccccc", font=("Consolas", 10))
log.pack(fill="both", expand=True, padx=8, pady=8)

def on_key(e):
    msg = (f"keysym={e.keysym!r:20} "
           f"char={e.char!r:6} "
           f"keycode={e.keycode:<6} "
           f"state=0x{e.state:08x} ({e.state})\n")
    log.insert("end", msg)
    log.see("end")
    return "break"

root.bind("<KeyPress>", on_key)
root.focus_force()
tk.Label(root, text="Press any key or combo — results appear below",
         bg="#1e1e1e", fg="#888888", font=("Segoe UI", 9)).pack()

root.mainloop()
