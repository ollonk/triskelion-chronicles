#!/usr/bin/env python3
import struct
import sys
from pathlib import Path

from PIL import Image


def encode_4bpp_tile(tile):
    out = bytearray()
    pixels = list(tile.getdata())
    for y in range(8):
        for x in range(0, 8, 2):
            left = pixels[y * 8 + x] & 0xF
            right = pixels[y * 8 + x + 1] & 0xF
            out.append(left | (right << 4))
    return bytes(out)


def main():
    if len(sys.argv) != 4:
        raise SystemExit("usage: build_pokelink_bg.py input.png output.4bpp output.bin")

    src = Path(sys.argv[1])
    out_gfx = Path(sys.argv[2])
    out_tilemap = Path(sys.argv[3])

    image = Image.open(src).convert("P")
    if image.size != (240, 160):
        raise SystemExit(f"{src} must be 240x160")

    unique_tiles = []
    tile_indexes = {}
    tilemap = [0] * (32 * 32)

    for ty in range(20):
        for tx in range(30):
            tile = image.crop((tx * 8, ty * 8, tx * 8 + 8, ty * 8 + 8))
            key = bytes(tile.tobytes())
            if key not in tile_indexes:
                tile_indexes[key] = len(unique_tiles)
                unique_tiles.append(tile)
            tilemap[ty * 32 + tx] = tile_indexes[key]

    if len(unique_tiles) > 512:
        raise SystemExit(f"{src} has {len(unique_tiles)} unique tiles; max is 512")

    out_gfx.write_bytes(b"".join(encode_4bpp_tile(tile) for tile in unique_tiles))
    out_tilemap.write_bytes(b"".join(struct.pack("<H", entry) for entry in tilemap))


if __name__ == "__main__":
    main()
