# Use to convert the images that will be displayed in the OS.
import sys
from PIL import Image

bmp_path = sys.argv[1]

# Convert to uncompressed BMP
uncompressed_path = sys.argv[2]
with Image.open(bmp_path) as img:
    img = img.resize((150, 150), Image.Resampling.LANCZOS)
    img = img.convert("RGBA")

    img.save(uncompressed_path, format="BMP")
