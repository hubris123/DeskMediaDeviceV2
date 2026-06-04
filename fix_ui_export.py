#!/usr/bin/env python3
"""
Run this after every SquareLine export to fix truncated files.
Usage: python fix_ui_export.py
"""
import re, os

BASE = os.path.dirname(os.path.abspath(__file__))
UI = os.path.join(BASE, 'components/squareline_ui/UIFiles')
HANDOFF = os.path.join(BASE, 'HANDOFF_FOLDER')

# ── GUI.h ──────────────────────────────────────────────────────────────────
path = os.path.join(UI, 'GUI.h')
with open(path, 'rb') as f:
    content = f.read().decode('utf-8', errors='replace')
content = content.replace('\x00', '').rstrip()
idx = content.rfind('#endif')
content = content[:idx] if idx >= 0 else content

REQUIRED = [
    ('fixed23',   'LV_FONT_DECLARE'),
    ('font',      'LV_FONT_DECLARE'),
    ('font_15',   'LV_FONT_DECLARE'),
    ('tiny',      'LV_FONT_DECLARE'),
    ('upload_34s_d824cbffe8e8400cabc43e41608fd72d_png', 'LV_IMG_DECLARE'),
    ('upload_35s_7a2b8dbb6e8a4c1d8a796fed8458d198_png', 'LV_IMG_DECLARE'),
    ('upload_hclbg1_52bba57ce173452fadd7595a14167a99_png', 'LV_IMG_DECLARE'),
]
for sym, macro in REQUIRED:
    if sym not in content:
        content += f'{macro}({sym});\n'
        print(f'  GUI.h: added {sym}')

content += '#endif /* _GUI_H */\n'
with open(path, 'w') as f:
    f.write(content)
print('✓ GUI.h fixed')

# ── home.c ─────────────────────────────────────────────────────────────────
path = os.path.join(UI, 'screens/home.c')
with open(path, 'rb') as f:
    content = f.read().decode('utf-8', errors='replace')
content = content.replace('\x00', '').rstrip()

# Remove truncated incomplete statements before closing brace
content = re.sub(r'\n\s*lv_\w*\s*\n}$', '\n}', content)
content = re.sub(r'\n  lv_label_set_text\([^;]*\n}$', '\n}', content)
content = re.sub(r'\n  lv_obj_add_style\([^;]*\n}$', '\n}', content, flags=re.DOTALL)
content = re.sub(r'\n  ui_object_set_themeable_style_property\([^\}]*\n}$', '\n}', content, flags=re.DOTALL)
content = re.sub(r'\n\s*\w\s*\n}$', '\n}', content)
if not content.endswith('}'):
    content += '\n}'

# Always replace styles function with known-good backup
with open(os.path.join(HANDOFF, 'home_styles_backup.c'), 'r') as f:
    styles = f.read()
if 'void GUI_initScreenStyles__home' in content:
    idx = content.find('void GUI_initScreenStyles__home')
    content = content[:idx].rstrip() + '\n\n' + styles
    print('✓ home.c: replaced truncated GUI_initScreenStyles__home with backup')
elif 'GUI_initScreenStyles__home' in content:
    content = content.rstrip() + '\n\n' + styles
    print('✓ home.c: appended GUI_initScreenStyles__home from backup')

with open(path, 'w') as f:
    f.write(content + '\n')
print('✓ home.c fixed')

# ── main.c null bytes ──────────────────────────────────────────────────────
path = os.path.join(BASE, 'main/main.c')
with open(path, 'rb') as f:
    raw = f.read()
cleaned = raw.replace(b'\x00', b'')
if len(raw) != len(cleaned):
    with open(path, 'wb') as f:
        f.write(cleaned)
    print(f'✓ main.c: removed {len(raw)-len(cleaned)} null bytes')

print('\nAll done. Run: idf.py build')
