"""
DeskMediaDevice — SD Card Content Converter (windowless launcher)

Double-click this .pyw file to start the converter with NO terminal window.
Windows runs .pyw files with pythonw.exe, which has no console.

This is just a launcher; all logic lives in sd_converter.py next to it.
"""

import os
import sys
import runpy

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
sys.argv = [os.path.join(BASE_DIR, "sd_converter.py")]
runpy.run_path(sys.argv[0], run_name="__main__")
