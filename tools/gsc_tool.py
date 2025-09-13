from PIL import Image

# 8 basic ANSI colors (0–7)
ANSI_COLORS = {
    0: (0, 0, 0),
    1: (128, 0, 0),
    2: (0, 128, 0),
    3: (128, 128, 0),
    4: (0, 0, 128),
    5: (128, 0, 128),
    6: (0, 128, 128),
    7: (192, 192, 192),
}

def closest_ansi(r, g, b):
    best_index = 0
    best_dist = float("inf")
    for idx, (cr, cg, cb) in ANSI_COLORS.items():
        dist = (r-cr)**2 + (g-cg)**2 + (b-cb)**2
        if dist < best_dist:
            best_dist = dist
            best_index = idx
    return best_index

def image_to_gsc(img_path, out_path):
    img = Image.open(img_path).convert("RGB")
    w, h = img.size

    with open(out_path, "w") as f:
        f.write(f"{w}\n{h}\n")
        for y in range(h):
            row_vals = []
            for x in range(w):
                r, g, b = img.getpixel((x, y))
                row_vals.append(str(closest_ansi(r, g, b)))
            f.write(" ".join(row_vals) + "\n")

if __name__ == "__main__":
    print("=== Image → GSC Converter ===")
    img_path = input("Enter path to image: ").strip()
    out_path = input("Enter output .gsc path (leave empty for default): ").strip()

    if not out_path:
        if "." in img_path:
            out_path = img_path.rsplit(".", 1)[0] + ".gsc"
        else:
            out_path = img_path + ".gsc"

    try:
        image_to_gsc(img_path, out_path)
        print(f"Done! Script saved to {out_path}")
    except Exception as e:
        print(f"Error: {e}")
