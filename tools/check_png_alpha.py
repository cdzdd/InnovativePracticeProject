from PIL import Image
import os
import sys

folder = os.path.join(os.path.dirname(__file__), '..', 'assets', 'enemyFish')
folder = os.path.abspath(folder)

if not os.path.isdir(folder):
    print('Folder not found:', folder)
    sys.exit(1)

files = sorted([f for f in os.listdir(folder) if f.lower().endswith(('.png','.gif','.jpg','.jpeg'))])
if not files:
    print('No image files found in', folder)
    sys.exit(0)

print('Checking images in', folder)
for fn in files:
    path = os.path.join(folder, fn)
    try:
        im = Image.open(path).convert('RGBA')
    except Exception as e:
        print(fn, '-> failed to open:', e)
        continue
    w,h = im.size
    px = im.getdata()
    total = w*h
    trans = 0
    nearly_white = 0
    for r,g,b,a in px:
        if a == 0:
            trans += 1
        # count very-light pixels (likely "white background")
        if r >= 250 and g >= 250 and b >= 250:
            nearly_white += 1
    print(f"{fn}: {w}x{h} transparent%={(trans/total*100):.2f} whiteish%={(nearly_white/total*100):.2f}")

print('Done')
