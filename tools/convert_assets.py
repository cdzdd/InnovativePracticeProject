#!/usr/bin/env python3
# convert_assets.py
# Scans assets/ for .gif and .jpg files and writes PNGs with an alpha mask
# - Converts white background to transparent
# - Leaves original files untouched; writes new files next to them with .png extension

from PIL import Image
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
ASSETS_DIRS = [ROOT / 'assets', ROOT / 'build' / 'assets']

# We will scan both the repo-level assets/ and build/assets/ directories (if present).

count = 0
for assets_dir in ASSETS_DIRS:
    if not assets_dir.exists():
        # skip missing asset copies
        continue
    for path in assets_dir.rglob('*'):
        if path.suffix.lower() in ('.gif', '.jpg', '.jpeg'):
            rel = path.relative_to(assets_dir)
            out = path.with_suffix('.png')
            # Overwrite existing PNGs to apply the improved background removal
            if out.exists():
                print(f"Overwriting: {rel} -> {out.name}")

            try:
                img = Image.open(path).convert('RGBA')
                w,h = img.size
                pixels = img.load()

                # Determine a flexible background removal by flood-filling from the border
                # This avoids turning interior off-white pixels into transparent when the
                # real background color is only along the edges.

                # Helper to test color similarity
                # use a stricter tolerance for enemy sprites so we don't remove
                # interior off-white/near-white pixels that are part of the fish
                tol = 40
                if 'enemyFish' in str(path):
                    tol = 10

                def similar(c1, c2, tol=tol):
                    return abs(c1[0]-c2[0])<=tol and abs(c1[1]-c2[1])<=tol and abs(c1[2]-c2[2])<=tol

                # take the majority color among border pixels as the background seed
                from collections import Counter, deque
                border_colors = []
                for x in range(w):
                    border_colors.append(img.getpixel((x,0))[:3])
                    border_colors.append(img.getpixel((x,h-1))[:3])
                for y in range(h):
                    border_colors.append(img.getpixel((0,y))[:3])
                    border_colors.append(img.getpixel((w-1,y))[:3])
                most_common = Counter(border_colors).most_common(1)
                if most_common:
                    bg_seed = most_common[0][0]
                else:
                    bg_seed = (255,255,255)

                visited = [[False]*h for _ in range(w)]
                q = deque()
                # initialize queue with border pixels similar to bg_seed
                for x in range(w):
                    for y in (0,h-1):
                        if not visited[x][y] and similar(img.getpixel((x,y))[:3], bg_seed, tol=40):
                            visited[x][y] = True
                            q.append((x,y))
                for y in range(h):
                    for x in (0,w-1):
                        if not visited[x][y] and similar(img.getpixel((x,y))[:3], bg_seed, tol=40):
                            visited[x][y] = True
                            q.append((x,y))

                # BFS to mark background-connected pixels
                while q:
                    x,y = q.popleft()
                    r,g,b,a = pixels[x,y]
                    pixels[x,y] = (r,g,b,0)
                    for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)):
                        nx,ny = x+dx, y+dy
                        if 0<=nx<w and 0<=ny<h and not visited[nx][ny]:
                            if similar(img.getpixel((nx,ny))[:3], bg_seed, tol=40):
                                visited[nx][ny] = True
                                q.append((nx,ny))
                # ensure directory exists for the matching assets dir
                out.parent.mkdir(parents=True, exist_ok=True)
                img.save(out, 'PNG')
                print(f"Converted: {path.relative_to(assets_dir)} -> {out.relative_to(assets_dir)}")
                count += 1
            except Exception as e:
                print(f"Failed to convert {rel}: {e}")

print(f"Done. {count} files converted.")
